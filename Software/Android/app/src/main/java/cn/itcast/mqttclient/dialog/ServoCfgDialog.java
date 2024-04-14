package cn.itcast.mqttclient.dialog;

import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import cn.itcast.mqttclient.MainActivity;
import cn.itcast.mqttclient.R;

public class ServoCfgDialog extends AlertDialog {

    private NumberPicker np_angle;
    private Button btn_center_angle,btn_set_angle,btn_save_angle;

    public ServoCfgDialog(Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_servo_cfg);
        initView();
        initEvent();
    }

    private void initView()
    {
        np_angle = (NumberPicker) findViewById(R.id.np_angle);
        np_angle.setMaxValue(180);
        np_angle.setMinValue(0);

        btn_center_angle = (Button) findViewById(R.id.btn_center_angle);
        btn_set_angle = (Button) findViewById(R.id.btn_set_angle);
        btn_save_angle = (Button) findViewById(R.id.btn_save_angle);
    }

    private void initEvent()
    {
        btn_center_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.TurnAngle(0);
            }
        });

        btn_set_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.TurnAngle(np_angle.getValue()-90); //-90~90
            }
        });

        btn_save_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.SaveAngle();
                dismiss();
            }
        });
    }


}
