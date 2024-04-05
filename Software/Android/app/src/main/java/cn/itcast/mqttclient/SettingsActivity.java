package cn.itcast.mqttclient;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;

import androidx.appcompat.app.AppCompatActivity;

public class SettingsActivity extends AppCompatActivity {

    private Button back_to_zero,set_turn_angle,save_turn_angle;
    private NumberPicker turn_angle;

    @SuppressLint("CutPasteId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        back_to_zero = (Button) findViewById(R.id.btn_back_to_zero);
        set_turn_angle = (Button) findViewById(R.id.btn_turn_angle);
        save_turn_angle = (Button) findViewById(R.id.btn_save_turn_angle);

        turn_angle = (NumberPicker) findViewById(R.id.np_angle);
        turn_angle.setMaxValue(90);
        turn_angle.setMinValue(0);

        back_to_zero.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.TurnAngle(45);
            }
        });
        set_turn_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.TurnAngle(turn_angle.getValue());
            }
        });
        save_turn_angle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.SaveAngle();
            }
        });
    }


}
