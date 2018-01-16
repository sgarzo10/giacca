package com.example.admin.appg;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Set;

public class AscoltatoreMainActivity implements View.OnClickListener {

    private MainActivity app;
    private boolean ricerca;
    private ArrayList<String> nomi = new ArrayList<String>();
    private ArrayList<String> mac = new ArrayList<String>();
    private BluetoothAdapter bluetoothAdapter;

    public void setRicerca(boolean ricerca) {this.ricerca = ricerca;}
    public ArrayList<String> getNomi() {return nomi;}
    public ArrayList<String> getMac() {return mac;}

    public AscoltatoreMainActivity(MainActivity app)
    {
        this.app=app;
        ricerca= false;
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        boolean acceso = bluetoothAdapter.isEnabled();
        if (acceso) {
            app.getCerca().setVisibility(View.VISIBLE);
            app.getAssociati().setVisibility(View.VISIBLE);
        }
    }

    public void inRicerca()
    {
        if (ricerca == true) {
            try {
                bluetoothAdapter.cancelDiscovery();
                app.getCerca().setImageDrawable(ContextCompat.getDrawable(app, R.drawable.lente));
                ricerca = false;
                app.getT().setText("Ricerca finita!");
            } catch (Exception e) {
                app.getT().setText("Errore");
            }
        }
    }
    public void dispositivi_associati()
    {
        inRicerca();
        try
        {
            Set<BluetoothDevice>pairedDevices;
            pairedDevices = bluetoothAdapter.getBondedDevices();
            if (pairedDevices.size()==0)
                app.getT().setText("Non ci sono dispositivi");
            else
            {
                app.getLinear().removeAllViews();
                mac.clear();
                nomi.clear();
                for (BluetoothDevice bt : pairedDevices) nomi.add(bt.getName());
                for (BluetoothDevice bt : pairedDevices) mac.add(bt.getAddress());
                app.getT().setText("Lista:");
                ArrayAdapter adapter = new ArrayAdapter(app, android.R.layout.simple_list_item_1, nomi);
                app.getLv().setAdapter(adapter);
                ArrayAdapter adapter1 = new ArrayAdapter(app, android.R.layout.simple_list_item_1, mac);
                app.getLv1().setAdapter(adapter1);
                for (int i=0; i<nomi.size();i++)
                {
                    Button pr= new Button(app);
                    pr.setId(R.id.elimina);
                    pr.setText("Elimina");
                    pr.setTag(i);
                    pr.setTextSize(10);
                    pr.setOnClickListener(this);
                    app.getLinear().addView(pr);
                }
            }
        }
        catch (NullPointerException e)
        {app.getT().setText("Non hai il bluetooth");}
    }

    public void accendi()
    {
        inRicerca();
        try {
            boolean acceso = bluetoothAdapter.isEnabled();
            if (!acceso) {
                Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                app.startActivityForResult(turnOn, 0);
                app.getT().setText("Acceso");
                app.getCerca().setVisibility(View.VISIBLE);
                app.getAssociati().setVisibility(View.VISIBLE);
            } else
                app.getT().setText("Già acceso");
        }
        catch (NullPointerException e)
        {app.getT().setText("Non hai il bluetooth");}
    }

    public void spegni()
    {
        inRicerca();
        try {
            bluetoothAdapter.disable();
            app.getCerca().setVisibility(View.INVISIBLE);
            app.getAssociati().setVisibility(View.INVISIBLE);
            app.getT().setText("Spento");
            app.getLinear().removeAllViews();
            mac.clear();
            nomi.clear();
        }
        catch (NullPointerException e)
        {app.getT().setText("Non hai il bluetooth");}
    }

    public void cerca()
    {
        if (ricerca == false)
        {
            mac.clear();
            nomi.clear();
            app.getLinear().removeAllViews();
            try {
                bluetoothAdapter.startDiscovery();
                app.getCerca().setImageDrawable(ContextCompat.getDrawable(app, R.drawable.stop));
                ricerca = true;
            }
            catch (Exception e)
            {app.getT().setText("Errore");}
        }
        else
            inRicerca();
    }

    public void connetti(int i)
    {
        inRicerca();
        Intent openPage1 = new Intent(app,ConnectionActivity.class);
        openPage1.putExtra("nome", nomi.get(i));
        openPage1.putExtra("mac", mac.get(i));
        mac.clear();
        nomi.clear();
        app.getLinear().removeAllViews();
        app.getT().setText("Mi sto connettendo...");
        app.startActivity(openPage1);
    }

    public void elimina(int i)
    {
        BluetoothDevice mmDevice=bluetoothAdapter.getRemoteDevice(mac.get(i));
        try {
            Method m = mmDevice.getClass().getMethod("removeBond", (Class[]) null);
            m.invoke(mmDevice, (Object[]) null);
            app.getT().setText("Dissociato");
            mac.clear();
            nomi.clear();
            app.getLinear().removeAllViews();
        } catch (Exception e) { app.getT().setText("Errore"); }
    }

    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.accendi:
                accendi();
                break;
            case R.id.spegni:
                spegni();
                break;
            case R.id.associati:
                dispositivi_associati();
                break;
            case R.id.cerca:
                cerca();
                break;
            case R.id.connetti:
                connetti((int) view.getTag());
                break;
            case R.id.elimina:
                elimina((int) view.getTag());
                break;
        }
    }
}
