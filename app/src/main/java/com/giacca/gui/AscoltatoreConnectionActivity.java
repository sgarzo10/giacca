package com.giacca.gui;

import android.Manifest;
import android.content.Context;
import android.location.LocationManager;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import com.giacca.R;
import java.util.Date;

public class AscoltatoreConnectionActivity implements View.OnClickListener {

    private ConnectionActivity app;
    private LocationManager locationManager;
    private MyLocationListener locationListener;

    AscoltatoreConnectionActivity(final ConnectionActivity app) {
        int permissionCheck = ContextCompat.checkSelfPermission(app, Manifest.permission.ACCESS_FINE_LOCATION);
        locationManager = (LocationManager) app.getSystemService(Context.LOCATION_SERVICE);
        this.app = app;
        locationListener = new MyLocationListener(app);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.invia:
                app.getBluetooth().invia(app.getE().getText().toString());
                break;
            case R.id.setData:
                Log.i("case","setData");
                app.getBluetooth().invia("data" + new Date().toString());
                break;
            case R.id.setGps:
                Log.i("case","startSetGps");
                app.getT().setText(app.getT().getText() + "\n" + "Provo a leggere GPS!");
                try {
                    int permissionCheck = ContextCompat.checkSelfPermission(app, Manifest.permission.ACCESS_FINE_LOCATION);
                    locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 5000, 5, locationListener);
                }
                catch (SecurityException s){
                    s.printStackTrace();
                }
                Log.i("stato","endSetGps");
                break;

        }
    }
}