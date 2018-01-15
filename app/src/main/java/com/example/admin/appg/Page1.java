package com.example.admin.appg;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import java.util.ArrayList;

public class Page1 extends AppCompatActivity {

    private AscoltatorePage1 ascoltatore;
    private BluetoothConnection bluetooth;
    private Button invia;
    private Button setData;
    private Button setGps;
    private EditText e;
    private TextView t;
    private TextView txtLa;
    private TextView txtLo;
    private ArrayList<View> views;

    public BluetoothConnection getBluetooth() { return bluetooth;}
    public Button getInvia() {
        return invia;
    }
    public EditText getE() {
        return e;
    }
    public TextView getT() {
        return t;
    }
    public TextView getTLa() {
        return txtLa;
    }
    public TextView getTLo() {
        return txtLo;
    }
    public ArrayList<View> getViews() {
        return views;
    }

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_page1);
        invia = (Button) findViewById(R.id.invia);
        setData = (Button) findViewById(R.id.setData);
        setGps = (Button) findViewById(R.id.setGps);
        txtLa = (TextView) findViewById(R.id.txtLat);
        txtLo = (TextView) findViewById(R.id.txtLo);
        e = (EditText) findViewById(R.id.inserisci);
        t = (TextView) findViewById(R.id.txtViewLog);
        views = new ArrayList<View>();
        String nome = getIntent().getExtras().getString("nome");
        String mac = getIntent().getExtras().getString("mac");
        ascoltatore = new AscoltatorePage1(this);
        bluetooth = new BluetoothConnection(this);
        invia.setOnClickListener(ascoltatore);
        setData.setOnClickListener(ascoltatore);
        setGps.setOnClickListener(ascoltatore);
        bluetooth.connetti(nome, mac);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    public void onStop() { super.onStop(); }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        bluetooth.disconnetti();
    }
}
