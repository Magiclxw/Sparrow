package com.project.sparrow;

import static com.project.sparrow.system.JsonHandler.analysisAppRetainedSettings;
import static com.project.sparrow.system.JsonHandler.getPowerOnOFF;
import static com.project.sparrow.system.JsonHandler.getWakeupInterval;

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
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;
import com.google.android.material.navigation.NavigationView;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.appcompat.app.AppCompatActivity;

import com.project.sparrow.ble.BLEDevice;
import com.project.sparrow.ble.BLEManager;
import com.project.sparrow.ble.LVDevicesAdapter;
import com.project.sparrow.ble.OnBleConnectListener;
import com.project.sparrow.ble.OnDeviceSearchListener;
import com.project.sparrow.ble.permission.PermissionListener;
import com.project.sparrow.ble.permission.PermissionRequestList;
import com.project.sparrow.databinding.ActivityMainBinding;
import com.project.sparrow.system.Mqtt;
import com.project.sparrow.system.SystemConfig;
import com.project.sparrow.system.TypeConversion;
import com.project.sparrow.system.UpdateState;
import com.project.sparrow.tools.KeyboardActivity;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration mAppBarConfiguration;
    private ActivityMainBinding binding;

    private static final String TAG = "BLEMain";
    private final String DeviceName = "Sparrow";
    private int quitFlag = 0; //退出确认标志
    public static String mqttServer;//ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
    public static String userName = "test";
    public static String password = "asd13579";   //用户密码
    public static String SERVICE_UUID = "000000ff-0000-1000-8000-00805f9b34fb";//"49535343-fe7d-4ae5-8fa9-9fafd205e455";  //蓝牙通讯服务
    public static String READ_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-1e4d-4bd9-ba61-23c647249616";  //读特征
    public static String WRITE_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-8841-43f4-a8d4-ecbe34729bb3";  //写特征

    public static int bleDataRecFlag = 0;
    public static byte bleRecData[];

    //动态申请权限
    private String[] requestPermissionArray = new String[]{
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.BLUETOOTH_SCAN,
            Manifest.permission.BLUETOOTH_ADVERTISE,
            Manifest.permission.BLUETOOTH_CONNECT
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


    static NumberPicker np_wakeup_hour;
    static NumberPicker np_wakeup_minute;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        mqttServer = SystemConfig.getMqttAddr(MainActivity.this);
        userName = SystemConfig.getMqttUserName(MainActivity.this);
        password = SystemConfig.getMqttPassword(MainActivity.this);
        SERVICE_UUID = SystemConfig.getBleServiceUUID(MainActivity.this);
        READ_UUID = SystemConfig.getBleReadUUID(MainActivity.this);
        WRITE_UUID = SystemConfig.getBleWriteUUID(MainActivity.this);

        if (mqttServer == null || mqttServer.equals("ssl://")) {
            mqttServer = "ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
            SystemConfig.setMqttAddr(this, mqttServer);
        }
        if (userName == null) {
            userName = "test";
            SystemConfig.setMqttUserName(this, userName);
        }
        if (password == null) {
            password = "asd13579";
            SystemConfig.setMqttPassword(this, password);
        }
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

        mContext = getApplicationContext();


        np_wakeup_hour = (NumberPicker) findViewById(R.id.np_wakeup_hour);
        np_wakeup_minute = (NumberPicker) findViewById(R.id.np_wakeup_minute);

        np_wakeup_hour.setMinValue(0);
        np_wakeup_hour.setMaxValue(240);
        np_wakeup_minute.setMinValue(0);
        np_wakeup_minute.setMaxValue(60);


        new Thread(new UpdateState()).start();

        //初始化数据
        initData();
        //注册广播
        initBLEBroadcastReceiver();
        initPermissions();
        searchBtDevice();

        Mqtt.connectServer(mqttServer, userName, password);
//        new Thread(new UpdateState()).start();

/*********************************************************************************************/
        setSupportActionBar(binding.appBarMain.toolbar);

        DrawerLayout drawer = binding.drawerLayout;
        NavigationView navigationView = binding.navView;
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        mAppBarConfiguration = new AppBarConfiguration.Builder(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow)
                .setOpenableLayout(drawer)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        NavigationUI.setupActionBarWithNavController(this, navController, mAppBarConfiguration);
        NavigationUI.setupWithNavController(navigationView, navController);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        return NavigationUI.navigateUp(navController, mAppBarConfiguration)
                || super.onSupportNavigateUp();
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
                switch (getPowerOnOFF())
                {
                    case 0:
                    {
//                        rb_no_ctrl.setChecked(true);
                        break;
                    }
                    case 1:
                    {
//                        rb_power_on.setChecked(true);
                        break;
                    }
                    case 2:
                    {
//                        rb_power_off.setChecked(true);
                    }
                }
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
}

