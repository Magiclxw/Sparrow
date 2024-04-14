package cn.itcast.mqttclient;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;

public class BlueToothController
{
    private static BluetoothAdapter bluetoothAdapter;

    public BlueToothController()
    {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    public static boolean isSupportBlueTooth() {
        if (bluetoothAdapter != null) {
            return true;
        } else {
            return false;
        }
    }

    public boolean getBlueToothStatus()
    {
        if(bluetoothAdapter != null)
        {
            return bluetoothAdapter.isEnabled();
        }
        return false;
    }

    public void enableVisibly(Context context)
    {
        Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION,300);
        context.startActivities(new Intent[]{intent});
    }
}
