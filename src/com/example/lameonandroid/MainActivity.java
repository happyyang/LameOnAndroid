package com.example.lameonandroid;

import java.io.File;

import android.os.Bundle;
import android.app.Activity;
import android.app.ProgressDialog;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {
    
    
    ProgressDialog progressDialog;

    public native String getLameVersion();

    public native void convert(String wavFile, String mp3File);

    static {
        System.loadLibrary( "lame");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState);
        setContentView( R.layout.activity_main);
        progressDialog = new ProgressDialog( this);
        progressDialog.setIndeterminate( false);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate( R.menu.main, menu);
        return true;
    }

    public void getVersion(View view) {
        Toast.makeText( this, "lame version:" + getLameVersion(), 1).show();
    }
    
    public void convert(View view){
        File file = new File("/sdcard/nbwgz.wav");
        progressDialog.setMax( (int)file.length());
        progressDialog.setProgressStyle( ProgressDialog.STYLE_HORIZONTAL);
        progressDialog.show();
        new Thread(){
            @Override
            public void run() {
                convert( "/sdcard/nbwgz.wav", "/sdcard/nbwgz.mp3");
            };
        }.start();
    }
    
    /**
     * 更新进度 ，共c语言调用
     * @param progress
     */
    public void updateProgress(int progress){
        progressDialog.setProgress( progress);
    }
}
