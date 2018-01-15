package com.example.admin.appg;

import android.widget.TextView;

public class MyHandler extends android.os.Handler{

    private StringBuilder sb = new StringBuilder();
    private Page1 app;

    public MyHandler(Page1 app) {this.app=app;}

    @Override
    public void handleMessage(android.os.Message msg) {
        switch (msg.what) {
            case 1:                                                   // if receive massage
                byte[] readBuf = (byte[]) msg.obj;
                String strIncom = new String(readBuf, 0, msg.arg1);                 // create string from bytes array
                sb.append(strIncom);                                               // append string
                int endOfLineIndex = sb.indexOf("\r\n");                            // determine the end-of-line
                if (endOfLineIndex > 0) {                                            // if end-of-line,
                    String sbprint = sb.substring(0, endOfLineIndex);               // extract string
                    sb.delete(0, sb.length());
                    String s=app.getT().getText()+"\n"+"RX: "+sbprint;
                    app.getT().setText(s);

                }
                //Log.d(TAG, "...String:"+ sb.toString() +  "Byte:" + msg.arg1 + "...");
                break;
        }
    }
}
