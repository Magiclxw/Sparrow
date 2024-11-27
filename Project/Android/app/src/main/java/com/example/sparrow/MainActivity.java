package com.example.sparrow;

import static com.example.sparrow.system.JsonHandler.analysisAppRetainedSettings;
import static com.example.sparrow.system.JsonHandler.analysisDevRetainedState;
import static com.example.sparrow.system.JsonHandler.getDeviceState;
import static com.example.sparrow.system.JsonHandler.getNextWakeupTime;
import static com.example.sparrow.system.JsonHandler.getWakeupInterval;
import static com.example.sparrow.system.JsonHandler.setPowerOnOff;
import static com.example.sparrow.system.JsonHandler.setWakeupInterval;
import static com.example.sparrow.system.Mqtt.mqttSendAppRetainedSettings;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.Menu;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;
import android.widget.Toast;

import com.example.sparrow.ble.BLEDevice;
import com.example.sparrow.ble.BLEManager;
import com.example.sparrow.ble.LVDevicesAdapter;
import com.example.sparrow.ble.OnBleConnectListener;
import com.example.sparrow.ble.OnDeviceSearchListener;
import com.example.sparrow.ble.permission.PermissionListener;
import com.example.sparrow.ble.permission.PermissionRequestList;
import com.example.sparrow.system.Mqtt;
import com.example.sparrow.system.SystemConfig;
import com.example.sparrow.system.TypeConversion;
import com.example.sparrow.system.UpdateState;


import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.appcompat.app.AppCompatActivity;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "BLEMain";
    private String DeviceName = "Sparrow";
    private int quitFlag = 0; //退出确认标志

    public static String SERVICE_UUID = "000000ff-0000-1000-8000-00805f9b34fb";//"49535343-fe7d-4ae5-8fa9-9fafd205e455";  //蓝牙通讯服务
    public static String READ_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-1e4d-4bd9-ba61-23c647249616";  //读特征
    public static String WRITE_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-8841-43f4-a8d4-ecbe34729bb3";  //写特征

    public static int bleDataRecFlag = 0;
    public static byte bleRecData[];

    //动态申请权限
    private String[] requestPermissionArray = new String[]{
            android.Manifest.permission.ACCESS_FINE_LOCATION,
            android.Manifest.permission.ACCESS_COARSE_LOCATION,
            android.Manifest.permission.BLUETOOTH_SCAN,
            android.Manifest.permission.BLUETOOTH_ADVERTISE,
            android.Manifest.permission.BLUETOOTH_CONNECT
    };
    // 声明一个集合，在后面的代码中用来存储用户拒绝授权的权限
    private List<String> deniedPermissionList = new ArrayList<>();

    //蓝牙
    private static final int CONNECT_SUCCESS = 0x01;
    private static final int CONNECT_FAILURE = 0x02;
    private static final int DISCONNECT_SUCCESS = 0x03;
    private static final int SEND_SUCCESS = 0x04;
    private static final int SEND_FAILURE = 0x05;
    private static final int RECEIVE_SUCCESS = 0x06;
    private static final int RECEIVE_FAILURE = 0x07;
    private static final int START_DISCOVERY = 0x08;
    private static final int STOP_DISCOVERY = 0x09;
    private static final int DISCOVERY_DEVICE = 0x0A;
    private static final int DISCOVERY_OUT_TIME = 0x0B;
    private static final int SELECT_DEVICE = 0x0C;
    private static final int BT_OPENED = 0x0D;
    private static final int BT_CLOSED = 0x0E;

    private BLEBroadcastReceiver bleBroadcastReceiver;
    private LVDevicesAdapter lvDevicesAdapter;
    public static BLEManager bleManager;
    private BluetoothDevice curBluetoothDevice;  //当前连接的设备

    public static Context mContext;

    public static Mqtt mqttHandle = null;

    static NumberPicker np_wakeup_hour;
    static NumberPicker np_wakeup_minute;
    static TextView tv_next_power_on_time;
    static TextView tv_device_state;

    static Button btn_power_on, btn_power_off ,btn_save_wakeup_interval;

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {

        if (item.getItemId() == R.id.action_tools)
        {
            Intent intent = new Intent();
            intent.setClass(MainActivity.this, ToolsActivity.class);
            startActivity(intent);
        }
        else if (item.getItemId() == R.id.action_settings)
        {
            Intent intent = new Intent();
            intent.setClass(MainActivity.this, SettingsActivity.class);
            startActivity(intent);
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressLint("ResourceType")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SERVICE_UUID = SystemConfig.getBleServiceUUID(MainActivity.this);
        READ_UUID = SystemConfig.getBleReadUUID(MainActivity.this);
        WRITE_UUID = SystemConfig.getBleWriteUUID(MainActivity.this);
        DeviceName = SystemConfig.getBleName(MainActivity.this);

        if (SERVICE_UUID == null) {
            SERVICE_UUID = "000000ff-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleServiceUUID(this, SERVICE_UUID);
        }
        if (READ_UUID == null) {
            READ_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleReadUUID(this, READ_UUID);
        }
        if (WRITE_UUID == null) {
            WRITE_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleReadUUID(this, WRITE_UUID);
        }
        if (DeviceName == null)
        {
            DeviceName = "Sparrow";
            SystemConfig.setBleName(this, DeviceName);
        }

        mContext = getApplicationContext();

        np_wakeup_hour = (NumberPicker) findViewById(R.id.np_wakeup_hour);
        np_wakeup_minute = (NumberPicker) findViewById(R.id.np_wakeup_minute);

        tv_next_power_on_time = (TextView) findViewById(R.id.next_power_on_time);
        tv_device_state = (TextView) findViewById(R.id.tv_device_state);


        btn_power_on = (Button) findViewById(R.id.btn_power_on);
        btn_power_off = (Button) findViewById(R.id.btn_power_off);
        btn_save_wakeup_interval = (Button) findViewById(R.id.btn_save_wakeup_interval);


        np_wakeup_hour.setMinValue(0);
        np_wakeup_hour.setMaxValue(240);
        np_wakeup_minute.setMinValue(0);
        np_wakeup_minute.setMaxValue(59);

        btn_power_on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setPowerOnOff(1);
                mqttSendAppRetainedSettings();
            }
        });

        btn_power_off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setPowerOnOff(2);
                mqttSendAppRetainedSettings();
            }
        });

        btn_save_wakeup_interval.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String wake_up_interval = String.valueOf(np_wakeup_hour.getValue()*60 + np_wakeup_minute.getValue());

                setWakeupInterval(Integer.parseInt(wake_up_interval));

                mqttSendAppRetainedSettings();
            }
        });

        new Thread(new UpdateState()).start();

        // 初始化数据
        initData();
        // 注册广播
        initBLEBroadcastReceiver();
        initPermissions();
        searchBtDevice();
        // 连接服务器
        mqttHandle.connectServer(MainActivity.this);
    }

    //mqtt通信
    @SuppressLint("HandlerLeak")
    public static Handler handler=new Handler(){
        @Override
        public void handleMessage(Message message){
            Bundle bundle=message.getData();

            String appRetainedSettings = bundle.getString(Mqtt.MQTT_TOPIC_APP_RETAINED_SETTINGS);
            String appDisretainedSettings = bundle.getString(Mqtt.MQTT_TOPIC_APP_DISRETAINED_SETTINGS);
            String deviceRetainedSettings = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_RETAINED_SETTINGS);
            String deviceDisretainedSettings = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS);
            String deviceRetainedState = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_RETAINED_STATE);
            String deviceDisretainedState = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_DISRETAINED_STATE);
            String deviceRetainedStatistics = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_RETAINED_STATISTICS);
            String deviceDisretainedStatistics = bundle.getString(Mqtt.MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS);

            if(appRetainedSettings != null){
                analysisAppRetainedSettings(appRetainedSettings);

                np_wakeup_hour.setValue(getWakeupInterval() / 60);
                np_wakeup_minute.setValue(getWakeupInterval() % 60);
            }
            if(appDisretainedSettings != null)
            {

            }
            if(deviceRetainedSettings != null)
            {

            }
            if(deviceDisretainedSettings != null)
            {

            }
            if(deviceRetainedState != null)
            {
                // 去除额外符号
                deviceRetainedState= deviceRetainedState.replaceAll("\\s*|\r|\n|\t","");

                System.out.println(deviceRetainedState);

                analysisDevRetainedState(deviceRetainedState);

                SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm");
                long originTime = getNextWakeupTime();
                String wakeupTime = sdf.format(new Date(originTime * 1000L));
                System.out.println("wakeup time : " + wakeupTime + "origin" + originTime);

                int deviceState = getDeviceState();
                if (deviceState == 0)
                {
                    tv_device_state.setTextColor(Color.parseColor("#ff0000"));
                    tv_device_state.setText("设备离线");
                    tv_next_power_on_time.setText(wakeupTime);
                }
                else
                {
                    tv_device_state.setTextColor(Color.parseColor("#00ff00"));
                    tv_device_state.setText("设备在线");
                    // 在线时不显示开机时间
                    tv_next_power_on_time.setText("");
                }
            }
        }
    };

    @SuppressLint("HandlerLeak")
    private Handler mHandler = new Handler() {
        @SuppressLint("SetTextI18n")
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            switch (msg.what) {
                case START_DISCOVERY:
                    Log.d(TAG, "开始搜索设备...");
                    break;

                case STOP_DISCOVERY:
                    Log.d(TAG, "停止搜索设备...");
                    break;

                case DISCOVERY_DEVICE:  //扫描到设备
                    BLEDevice bleDevice = (BLEDevice) msg.obj;
                    //lvDevicesAdapter.addDevice(bleDevice);
                    if (ActivityCompat.checkSelfPermission(getApplicationContext(), Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                        // TODO: Consider calling
                        //    ActivityCompat#requestPermissions
                        // here to request the missing permissions, and then overriding
                        //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
                        //                                          int[] grantResults)
                        // to handle the case where the user grants the permission. See the documentation
                        // for ActivityCompat#requestPermissions for more details.
//                        return;
                    }
                    String deviceName = bleDevice.getBluetoothDevice().getName();
                    System.out.println("device name : " + deviceName);
                    if (deviceName != null && deviceName.equals(DeviceName) )
                    {
                        curBluetoothDevice = bleDevice.getBluetoothDevice();
                        bleManager.stopDiscoveryDevice();
                        if(bleManager != null){
                            bleManager.connectBleDevice(mContext,curBluetoothDevice,15000,SERVICE_UUID,READ_UUID,WRITE_UUID,onBleConnectListener);
                        }
                        Log.d(TAG, "找到Sparrow");
                    }
                    else
                    {
                        Log.d(TAG, "没有找到Sparrow");
                    }
                    break;

                case SELECT_DEVICE:
                    break;

                case CONNECT_FAILURE: //连接失败
                    Log.d(TAG, "连接失败");
                    break;

                case CONNECT_SUCCESS:  //连接成功
                    Log.d(TAG, "连接成功");
                    break;

                case DISCONNECT_SUCCESS:
                    Log.d(TAG, "断开成功");
                    searchBtDevice();//重新建立连接
                    //startConnectionMonitoring();
                    break;

                case SEND_FAILURE: //发送失败
                    byte[] sendBufFail = (byte[]) msg.obj;
                    String sendFail = TypeConversion.bytes2HexString(sendBufFail,sendBufFail.length);
                    Toast.makeText(MainActivity.this,"发送数据失败，长度" + sendBufFail.length + "--> " + sendFail,Toast.LENGTH_LONG);
                    break;

                case SEND_SUCCESS:  //发送成功
                    byte[] sendBufSuc = (byte[]) msg.obj;
                    String sendResult = TypeConversion.bytes2HexString(sendBufSuc,sendBufSuc.length);
                    Toast.makeText(MainActivity.this,"发送数据成功，长度" + sendBufSuc.length + "--> " + sendResult,Toast.LENGTH_LONG);;
                    break;

                case RECEIVE_FAILURE: //接收失败
                    String receiveError = (String) msg.obj;
                    Toast.makeText(MainActivity.this,receiveError,Toast.LENGTH_LONG);;
                    break;

                case RECEIVE_SUCCESS:  //接收成功
                    byte[] recBufSuc = (byte[]) msg.obj;
                    String receiveResult = TypeConversion.bytes2HexString(recBufSuc,recBufSuc.length);
                    System.out.println("main rec data");
                    break;

                case BT_CLOSED:
                    Log.d(TAG, "系统蓝牙已关闭");
                    break;

                case BT_OPENED:
                    Log.d(TAG, "系统蓝牙已打开");
                    break;
                case DISCOVERY_OUT_TIME:
                    searchBtDevice();
                    break;
            }

        }
    };

    private void searchBtDevice() {
        if(bleManager == null){
            Log.d(TAG, "searchBtDevice()-->bleManager == null");
            return;
        }

        if (bleManager.isDiscovery()) { //当前正在搜索设备...
            bleManager.stopDiscoveryDevice();
        }

        if(lvDevicesAdapter != null){
            lvDevicesAdapter.clear();  //清空列表
        }

        //开始搜索
        bleManager.startDiscoveryDevice(onDeviceSearchListener,15000);
    }
    //扫描结果回调
    private OnDeviceSearchListener onDeviceSearchListener = new OnDeviceSearchListener() {

        @Override
        public void onDeviceFound(BLEDevice bleDevice) {
            Message message = new Message();
            message.what = DISCOVERY_DEVICE;
            message.obj = bleDevice;
            mHandler.sendMessage(message);
        }

        @Override
        public void onDiscoveryOutTime() {
            Message message = new Message();
            message.what = DISCOVERY_OUT_TIME;
            mHandler.sendMessage(message);
        }
    };

    /**
     * 初始化数据
     */
    private void initData() {
        //初始化ble管理器
        bleManager = new BLEManager();
        if(!bleManager.initBle(mContext)) {
            Log.d(TAG, "该设备不支持低功耗蓝牙");
            Toast.makeText(mContext, "该设备不支持低功耗蓝牙", Toast.LENGTH_SHORT).show();
        }else{
            if(!bleManager.isEnable()){
                //去打开蓝牙
                bleManager.openBluetooth(mContext,false);
            }
        }
    }

    /**
     * 注册广播
     */
    private void initBLEBroadcastReceiver() {
        //注册广播接收
        bleBroadcastReceiver = new BLEBroadcastReceiver();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED); //开始扫描
        intentFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);//扫描结束
        intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);//手机蓝牙状态监听
        registerReceiver(bleBroadcastReceiver,intentFilter);
    }

    /**
     * 初始化权限
     */
    private void initPermissions() {
        //Android 6.0以上动态申请权限
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            final PermissionRequestList permissionRequest = new PermissionRequestList();
            permissionRequest.requestRuntimePermission(MainActivity.this, requestPermissionArray, new PermissionListener() {
                @Override
                public void onGranted() {
                    Log.d(TAG,"所有权限已被授予");
                }

                //用户勾选“不再提醒”拒绝权限后，关闭程序再打开程序只进入该方法！
                @Override
                public void onDenied(List<String> deniedPermissions) {
                    deniedPermissionList = deniedPermissions;
                    for (String deniedPermission : deniedPermissionList) {
                        Log.e(TAG,"被拒绝权限：" + deniedPermission);
                    }
                }
            });
        }
    }
    //连接回调
    private OnBleConnectListener onBleConnectListener = new OnBleConnectListener() {
        @Override
        public void onConnecting(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice) {

        }

        @Override
        public void onConnectSuccess(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, int status) {
            //因为服务发现成功之后，才能通讯，所以在成功发现服务的地方表示连接成功
        }

        @Override
        public void onConnectFailure(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, String exception, int status) {
            Message message = new Message();
            message.what = CONNECT_FAILURE;
            mHandler.sendMessage(message);
        }

        @Override
        public void onDisConnecting(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice) {

        }

        @Override
        public void onDisConnectSuccess(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, int status) {
            Message message = new Message();
            message.what = DISCONNECT_SUCCESS;
            message.obj = status;
            mHandler.sendMessage(message);
        }

        @Override
        public void onServiceDiscoverySucceed(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, int status) {
            //因为服务发现成功之后，才能通讯，所以在成功发现服务的地方表示连接成功
            Message message = new Message();
            message.what = CONNECT_SUCCESS;
            mHandler.sendMessage(message);
        }

        @Override
        public void onServiceDiscoveryFailed(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, String failMsg) {
            Message message = new Message();
            message.what = CONNECT_FAILURE;
            mHandler.sendMessage(message);
        }

        @Override
        public void onReceiveMessage(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, BluetoothGattCharacteristic characteristic, byte[] msg) {
            Message message = new Message();
            message.what = RECEIVE_SUCCESS;
            message.obj = msg;
            mHandler.sendMessage(message);
            bleRecData = msg;
            bleDataRecFlag = 1;
        }

        @Override
        public void onReceiveError(String errorMsg) {
            Message message = new Message();
            message.what = RECEIVE_FAILURE;
            mHandler.sendMessage(message);
        }

        @Override
        public void onWriteSuccess(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, byte[] msg) {
            Message message = new Message();
            message.what = SEND_SUCCESS;
            message.obj = msg;
            mHandler.sendMessage(message);
        }

        @Override
        public void onWriteFailure(BluetoothGatt bluetoothGatt, BluetoothDevice bluetoothDevice, byte[] msg, String errorMsg) {
            Message message = new Message();
            message.what = SEND_FAILURE;
            message.obj = msg;
            mHandler.sendMessage(message);
        }

        @Override
        public void onReadRssi(BluetoothGatt bluetoothGatt, int Rssi, int status) {

        }

        @Override
        public void onMTUSetSuccess(String successMTU, int newMtu) {

        }

        @Override
        public void onMTUSetFailure(String failMTU) {

        }
    };
    /**
     * 蓝牙广播接收器
     */
    private class BLEBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (TextUtils.equals(action, BluetoothAdapter.ACTION_DISCOVERY_STARTED)) { //开启搜索
                Message message = new Message();
                message.what = START_DISCOVERY;
                mHandler.sendMessage(message);

            } else if (TextUtils.equals(action, BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {//完成搜素
                Message message = new Message();
                message.what = STOP_DISCOVERY;
                mHandler.sendMessage(message);

            } else if(TextUtils.equals(action,BluetoothAdapter.ACTION_STATE_CHANGED)){   //系统蓝牙状态监听

                int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,0);
                if(state == BluetoothAdapter.STATE_OFF){
                    Message message = new Message();
                    message.what = BT_CLOSED;
                    mHandler.sendMessage(message);

                }else if(state == BluetoothAdapter.STATE_ON){
                    Message message = new Message();
                    message.what = BT_OPENED;
                    mHandler.sendMessage(message);

                }
            }
        }
    }
    // 添加菜单
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main,menu);
        return true;
    }
    public static void mainConnectServer()
    {
        mqttHandle.connectServer(mContext);
    }

}