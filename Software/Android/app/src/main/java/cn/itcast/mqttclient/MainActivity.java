  package cn.itcast.mqttclient;

import static cn.itcast.mqttclient.util.JsonHandler.analysisAppRetainedSettings;
import static cn.itcast.mqttclient.util.JsonHandler.generateAppRetainedSettings;
import static cn.itcast.mqttclient.util.JsonHandler.getLedCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPassword;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordCtrl;
import static cn.itcast.mqttclient.util.JsonHandler.getPcPasswordWait;
import static cn.itcast.mqttclient.util.JsonHandler.getPowerOnOFF;
import static cn.itcast.mqttclient.util.JsonHandler.getToken;
import static cn.itcast.mqttclient.util.JsonHandler.getWakeupInterval;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.accessibilityservice.AccessibilityService;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.ArrayList;
import java.util.List;

import cn.itcast.mqttclient.ble.BLEInterface;
import cn.itcast.mqttclient.ble.BLEManager;
import cn.itcast.mqttclient.ble.OnBleConnectListener;
import cn.itcast.mqttclient.ble.OnDeviceSearchListener;
import cn.itcast.mqttclient.permission.PermissionListener;
import cn.itcast.mqttclient.permission.PermissionRequestList;
import cn.itcast.mqttclient.toolsActivity.UploadFileActivity;
import cn.itcast.mqttclient.toolsActivity.UploadTextActivity;
import cn.itcast.mqttclient.util.TypeConversion;


  @RequiresApi(api = Build.VERSION_CODES.S)
  public class MainActivity extends AppCompatActivity{

    private static final String TAG = "BLEMain";
    private final String DeviceName = "Sparrow";
    private int quitFlag=0; //退出确认标志
    static String mqttServer;//ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
    public static String userName = "test";
    public static String password = "asd13579";   //用户密码
    public static  String SERVICE_UUID = "000000ff-0000-1000-8000-00805f9b34fb";//"49535343-fe7d-4ae5-8fa9-9fafd205e455";  //蓝牙通讯服务
    public static  String READ_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-1e4d-4bd9-ba61-23c647249616";  //读特征
    public static  String WRITE_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";//"49535343-8841-43f4-a8d4-ecbe34729bb3";  //写特征

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

    /* 主题 */
    public static String MQTT_TOPIC_APP_RETAINED_SETTINGS = "/settings/app_config/retained";
    public static String MQTT_TOPIC_APP_DISRETAINED_SETTINGS = "/settings/app_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_SETTINGS  = "/settings/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS = "/settings/device_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_STATE = "/state/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_STATE = "/state/device_config/disretained";
    public static String MQTT_TOPIC_DEVICE_RETAINED_STATISTICS = "/statistics/device_config/retained";
    public static String MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS = "/statistics/device_config/disretained";

    private static final int CONNECT_SUCCESS = 0x01;
    private static final int CONNECT_FAILURE = 0x02;
    private static final int DISCONNECT_SUCCESS = 0x03;
    private static final int SEND_SUCCESS = 0x04;
    private static final int SEND_FAILURE= 0x05;
    private static final int RECEIVE_SUCCESS= 0x06;
    private static final int RECEIVE_FAILURE =0x07;
    private static final int START_DISCOVERY = 0x08;
    private static final int STOP_DISCOVERY = 0x09;
    private static final int DISCOVERY_DEVICE = 0x0A;
    private static final int DISCOVERY_OUT_TIME = 0x0B;
    private static final int SELECT_DEVICE = 0x0C;
    private static final int BT_OPENED = 0x0D;
    private static final int BT_CLOSED = 0x0E;

    private Button saveConfig,btn_tools,apply,settings,Btn_test;

    static TextView connectState,powerState,powerValue;
    private static RadioGroup power_contral;
    private static RadioButton rb_no_ctrl,rb_power_on,rb_power_off;
    private static NumberPicker numberPicker_wake_hour;
    private static NumberPicker numberPicker_wake_minute;
    private static NumberPicker numberPicker_wakeup_duragion;

    public static MqttClient client = null;

    //蓝牙
    private BLEBroadcastReceiver bleBroadcastReceiver;
    private LVDevicesAdapter lvDevicesAdapter;
    public static BLEManager bleManager;
    private BluetoothDevice curBluetoothDevice;  //当前连接的设备

    public static Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mqttServer = SystemConfig.getMqttAddr(MainActivity.this);
        userName = SystemConfig.getMqttUserName(MainActivity.this);
        password = SystemConfig.getMqttPassword(MainActivity.this);
        SERVICE_UUID = SystemConfig.getBleServiceUUID(MainActivity.this);
        READ_UUID = SystemConfig.getBleReadUUID(MainActivity.this);
        WRITE_UUID = SystemConfig.getBleWriteUUID(MainActivity.this);

        bleRecData = new byte[1024];

        if(mqttServer == null || mqttServer.equals("ssl://"))
        {
            mqttServer = "ssl://j1aa1aff.ala.cn-hangzhou.emqxsl.cn:8883";
            SystemConfig.setMqttAddr(this,mqttServer);
        }
        if(userName == null)
        {
            userName = "test";
            SystemConfig.setMqttUserName(this,userName);
        }
        if(password == null)
        {
            password = "asd13579";
            SystemConfig.setMqttPassword(this,password);
        }
        if(SERVICE_UUID == null)
        {
            SERVICE_UUID = "000000ff-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleServiceUUID(this,SERVICE_UUID);
        }
        if(READ_UUID == null)
        {
            READ_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleReadUUID(this,READ_UUID);
        }
        if(WRITE_UUID == null)
        {
            WRITE_UUID = "0000ff01-0000-1000-8000-00805f9b34fb";
            SystemConfig.setBleReadUUID(this,WRITE_UUID);
        }

        mContext = getApplicationContext();

        //按键
        saveConfig = (Button) findViewById(R.id.save_config);
        btn_tools = (Button) findViewById(R.id.btn_tools);
        settings = (Button)findViewById(R.id.btn_settings);
        Btn_test = (Button) findViewById(R.id.btn_test);

        //数字选择器
        numberPicker_wake_hour = (NumberPicker)findViewById(R.id.wakeup_interval_hour);
        numberPicker_wake_hour.setMinValue(0);
        numberPicker_wake_hour.setMaxValue(100);
        //numberPicker_wake_hour.setWrapSelectorWheel(false);
        numberPicker_wake_minute = (NumberPicker)findViewById(R.id.wakeup_interval_minute);
        numberPicker_wake_minute.setMinValue(0);
        numberPicker_wake_minute.setMaxValue(59);
        //numberPicker_wake_minute.setWrapSelectorWheel(false);
        numberPicker_wakeup_duragion = (NumberPicker)findViewById(R.id.wakeup_duration_minute);
        numberPicker_wakeup_duragion.setMinValue(1);
        numberPicker_wakeup_duragion.setMaxValue(59);
        //numberPicker_wakeup_duragion.setWrapSelectorWheel(false);
        //文本
        connectState = (TextView)findViewById(R.id.connect_state);
        powerState = (TextView)findViewById(R.id.power_state);
        powerValue = (TextView)findViewById(R.id.power_value);

        //单选按钮
        power_contral = (RadioGroup)findViewById(R.id.rd_power_ctrl);
        rb_no_ctrl = (RadioButton) findViewById(R.id.radio_no_exe);
        rb_power_on = (RadioButton) findViewById(R.id.radio_power_on);
        rb_power_off = (RadioButton) findViewById(R.id.radio_power_off);

        //蓝牙配置
        //iniListener();
        //初始化数据
        initData();
        //注册广播
        initBLEBroadcastReceiver();
        initPermissions();

        searchBtDevice();

        new Thread(new UpdateState()).start();

        String broker = mqttServer;
        String clientId = "Controller";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            client = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setUserName(userName);
            connOpts.setPassword(password.toCharArray());
            connOpts.setCleanSession(true);
            // 设置回调
            client.setCallback(new OnMessageCallback());
            // 建立连接
            System.out.println("Connecting to broker: " + broker);
            try {
                client.connect(connOpts);
                Toast.makeText(MainActivity.this,"服务器连接成功",Toast.LENGTH_SHORT).show();
            } catch (MqttException e) {
                Toast.makeText(MainActivity.this,"服务器连接失败",Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
            //订阅唤醒间隔主题
            try {
                client.subscribe(MQTT_TOPIC_APP_RETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅唤醒时长主题
            try {
                client.subscribe(MQTT_TOPIC_APP_DISRETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源控制主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅连接状态主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            //订阅电源状态主题
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_STATE);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_STATE);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_RETAINED_STATISTICS);

            } catch (MqttException e) {
                e.printStackTrace();
            }
            try {
                client.subscribe(MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS);

            } catch (MqttException e) {
                e.printStackTrace();
            }

        } catch (MqttException me) {
            System.out.println("reason " + me.getReasonCode());
            System.out.println("msg " + me.getMessage());
            System.out.println("loc " + me.getLocalizedMessage());
            System.out.println("cause " + me.getCause());
            System.out.println("excep " + me);
            me.printStackTrace();
        }

        saveConfig.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int qos = 1;
                int checkedRadioButton;
                String wake_up_interval = String.valueOf(numberPicker_wake_hour.getValue()*60 + numberPicker_wake_minute.getValue());

                //查找被选中的RadioButton
                for(checkedRadioButton = 0 ;checkedRadioButton < power_contral.getChildCount();checkedRadioButton++) {
                    RadioButton rb = (RadioButton) power_contral.getChildAt(checkedRadioButton);
                    if (rb.isChecked()) {
                        break;
                    }
                }

                String jsonData = generateAppRetainedSettings(Integer.parseInt(wake_up_interval), checkedRadioButton, getPcPassword(), getPcPasswordCtrl(), getPcPasswordWait(), getLedCtrl(), getToken());

                MqttMessage message = new MqttMessage(jsonData.getBytes());
                message.setQos(qos);
                try {
                    client.publish(MQTT_TOPIC_APP_RETAINED_SETTINGS, message);
                    System.out.println("Message published");
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });
        Btn_test.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //getTextFromClip(MainActivity.this);
                short data[] = new short[30];
//                data[0] = 0xC0;
//                data[1] = 0x0C;
//                data[2] = 0x01;
//                data[3] = 0x02;
//                data[4] = 0x00;
//                data[5] = 0x01;
//                data[6] = 0x04;
//                data[7] = 0xED;
//                data[8] = 0xDE;
//                data[9] = 0x01;
//                for(int i=0; i< 30; i++)
//                {
//                    data[i] = (short) i;
//                }
//            BLEInterface.cmdHidDataSend(data,(byte) 30);
            }
        });

        btn_tools.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, ToolsActivity.class);
                startActivity(intent);
            }
        });



        settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this,SettingsActivity.class);
                startActivity(intent);
            }
        });

        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_USER_PRESENT);
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        LockScreenReceiver receiver = new LockScreenReceiver();
        registerReceiver(receiver, filter);
    }

    @SuppressLint("HandlerLeak")
    public static Handler handler=new Handler(){
        @Override
        public void handleMessage(Message message){
            Bundle bundle=message.getData();

            String appRetainedSettings = bundle.getString(MQTT_TOPIC_APP_RETAINED_SETTINGS);
            String appDisretainedSettings = bundle.getString(MQTT_TOPIC_APP_DISRETAINED_SETTINGS);
            String deviceRetainedSettings = bundle.getString(MQTT_TOPIC_DEVICE_RETAINED_SETTINGS);
            String deviceDisretainedSettings = bundle.getString(MQTT_TOPIC_DEVICE_DISRETAINED_SETTINGS);
            String deviceRetainedState = bundle.getString(MQTT_TOPIC_DEVICE_RETAINED_STATE);
            String deviceDisretainedState = bundle.getString(MQTT_TOPIC_DEVICE_DISRETAINED_STATE);
            String deviceRetainedStatistics = bundle.getString(MQTT_TOPIC_DEVICE_RETAINED_STATISTICS);
            String deviceDisretainedStatistics = bundle.getString(MQTT_TOPIC_DEVICE_DISRETAINED_STATISTICS);

            if(appRetainedSettings != null){
                analysisAppRetainedSettings(appRetainedSettings);

                numberPicker_wake_hour.setValue(getWakeupInterval() / 60);
                numberPicker_wake_minute.setValue(getWakeupInterval() % 60);
                switch (getPowerOnOFF())
                {
                    case 0:
                    {
                        rb_no_ctrl.setChecked(true);
                        break;
                    }
                    case 1:
                    {
                        rb_power_on.setChecked(true);
                        break;
                    }
                    case 2:
                    {
                        rb_power_off.setChecked(true);
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
    private Handler mHandler = new Handler(){
        @SuppressLint("SetTextI18n")
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            switch(msg.what){
                case START_DISCOVERY:
                    Log.d(TAG, "开始搜索设备...");
                    break;

                case STOP_DISCOVERY:
                    Log.d(TAG, "停止搜索设备...");
                    break;

                case DISCOVERY_DEVICE:  //扫描到设备
                    BLEDevice bleDevice = (BLEDevice) msg.obj;
                    //lvDevicesAdapter.addDevice(bleDevice);
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
       * 初始化监听
       */
//      private void iniListener() {
//          btSearch.setOnClickListener(this);
//          btConnect.setOnClickListener(this);
//          btDisconnect.setOnClickListener(this);
//          btSend.setOnClickListener(this);
//
//          lvDevices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//              @Override
//              public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
//                  BLEDevice bleDevice = (BLEDevice) lvDevicesAdapter.getItem(i);
//                  BluetoothDevice bluetoothDevice = bleDevice.getBluetoothDevice();
//                  if(bleManager != null){
//                      bleManager.stopDiscoveryDevice();
//                  }
//                  Message message = new Message();
//                  message.what = SELECT_DEVICE;
//                  message.obj = bluetoothDevice;
//                  mHandler.sendMessage(message);
//              }
//          });
//      }

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

    public void onBackPressed(){

        if(quitFlag==1){
            MainActivity.this.finish();
        }
        if(quitFlag==0) {
            Toast.makeText(MainActivity.this, "再次返回退出", Toast.LENGTH_LONG).show();
            quitFlag++;
        }
    }
    public static void getTextFromClip(Context context){
      ClipboardManager clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
      //判断剪切版时候有内容
      if(!clipboardManager.hasPrimaryClip())
          return;
      ClipData clipData = clipboardManager.getPrimaryClip();
      //获取 ClipDescription
      ClipDescription clipDescription = clipboardManager.getPrimaryClipDescription();
      //获取 lable
      String lable = clipDescription.getLabel().toString();
      //获取 text
      String text = clipData.getItemAt(0).getText().toString();

      System.out.println(text);
    }
    public class LockScreenReceiver extends BroadcastReceiver {

      @Override
      public void onReceive(Context context, Intent intent) {
          if (intent.getAction().equals(Intent.ACTION_USER_PRESENT)) {
              // 屏幕解锁
              // 在这里处理解锁事件
              BLEInterface.cmdKeyboardFunc((byte)0,(byte)0x28);
          } else if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
              // 屏幕锁屏
              // 在这里处理锁屏事件
              BLEInterface.cmdKeyboardFunc((byte)0,(byte)0x28);
          }
      }
    }
}






