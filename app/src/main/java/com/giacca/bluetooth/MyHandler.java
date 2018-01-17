package com.giacca.bluetooth;

import com.giacca.gui.ConnectionActivity;

public class MyHandler extends android.os.Handler{

    private StringBuilder sb;
    private ConnectionActivity app;

    MyHandler(ConnectionActivity app) {this.app=app;
        sb = new StringBuilder();
    }

    @Override
    public void handleMessage(android.os.Message msg) {
        switch (msg.what) {
            case 1:
                byte[] readBuf = (byte[]) msg.obj;
                String strIncom = new String(readBuf, 0, msg.arg1);
                sb.append(strIncom);
                int endOfLineIndex = sb.indexOf("\r\n");
                if (endOfLineIndex > 0) {
                    String sbprint = sb.substring(0, endOfLineIndex);
                    sb.delete(0, sb.length());
                    String s=app.getT().getText()+"\n"+"RX: "+sbprint;
                    app.getT().setText(s);

                }
                break;
        }
    }
}
