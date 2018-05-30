package com.example.bluetooth;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

/*
 *
 * 
 * */
class bluetoothMsgThread extends Thread {
	private DataInputStream mmInStream;
	private Handler msgHandler;
	public bluetoothMsgThread(DataInputStream mmInStream,Handler msgHandler) {
		this.mmInStream = mmInStream;
		this.msgHandler = msgHandler;
	}
	
	public void run() {
		byte[] InBuffer = new byte[15];
		while (true) {
		  try {
			  Message msg = new Message();
			  msg.arg1 =mmInStream.read(InBuffer, 0, 15);

		    msg.what = 0x1234;
		    msg.obj = InBuffer;

		    msgHandler.sendMessage(msg);
		  }catch(IOException e) {
			  e.printStackTrace();
		  }
		}

//		byte[] InBuffer = new byte[15];           //创建 缓冲区
//		while (!Thread.interrupted()) {
//			try {
//				mmInStream.readFully(InBuffer, 0, 15); //读取蓝牙数据流
//				Message msg = new Message();          //定义一个消息,并填充数据
//				msg.what = 0x1234;
//				msg.obj = InBuffer;
//				msg.arg1 = InBuffer.length;
//				msgHandler.sendMessage(msg);          //通过handler发送消息
//			}catch(IOException e) {
//				e.printStackTrace();
//			}
//		}
	}
}

public class MainActivity extends Activity {
	BluetoothAdapter mBluetoothAdapter;
	BluetoothDevice device;
	BluetoothSocket clientSocket;
	BluetoothServerSocket btserver;
	String address;
	OutputStream mmOutStream;
	DataInputStream mmInStream;
	bluetoothMsgThread blue_tooth_msg_thread;

	EditText editText;

	UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	


	public void show_result(byte[] buffer,int count)
	{
		StringBuffer msg = new StringBuffer();
		StringBuffer msg2 = new StringBuffer();
		TextView tvInfo = findViewById(R.id.textViewReceiveInfo);
		tvInfo.setText("");
		for (int i = 0; i < count; i++) {
		  msg.append(String.format("0x%x ", buffer[i]));
		  msg2.append(String.format("%c", buffer[i]));
		}
		msg.append("\r\n");
		msg.append(msg2);
		tvInfo.setText(msg);
	}
	
	/*
	 *   ??????????,?????????????? ???????
	 * 
	 * */
	public void set_btn_status(boolean status)
	{
		Button ledonBtn = (Button)findViewById(R.id.headBtn);
		ledonBtn.setEnabled(status);		
		Button ledoffBtn = (Button)findViewById(R.id.retreatBtn);
		ledoffBtn.setEnabled(status);
		Button jdqonBtn = (Button)findViewById(R.id.leftBtn);
		jdqonBtn.setEnabled(status);
		Button jdqoffBtn = (Button)findViewById(R.id.rightBtn);
		jdqoffBtn.setEnabled(status);

		Button speedBtn=findViewById(R.id.sendBtn);
		speedBtn.setEnabled(status);
	}
	
	protected void onDestroy() {
		super.onDestroy();		
		try {
		  if (mmOutStream != null)           
		    mmOutStream.close();
		  if (mmInStream != null)
			  mmInStream.close();
		  if (clientSocket != null)
		    clientSocket.close();
		  blue_tooth_msg_thread.interrupt();
		 // Toast.makeText(getApplicationContext(), "?????????????????", Toast.LENGTH_LONG).show();
		}catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initPression();
		set_btn_status(false);
		
		editText=findViewById(R.id.edtv_speed);
		
		Button searchDeviceBtn = (Button)findViewById(R.id.searchDeviceBtn);
		searchDeviceBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
		       	mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		       	if (mBluetoothAdapter == null) {
		       		Toast.makeText(getApplicationContext(), "bluetooth is no available",Toast.LENGTH_LONG).show();
		       		finish();
		       		return;
		       	}
		       	
		       	mBluetoothAdapter.enable();
		       	if (!mBluetoothAdapter.isEnabled()) {
		       		Toast.makeText(getApplicationContext(), "bluetooth function is no available",Toast.LENGTH_LONG).show();
		       		finish();
		       		return;
		       	}
		       	
		       	
		        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
		       	if (pairedDevices.size() < 1) {
		       		Toast.makeText(getApplicationContext(), "未获取到配对设备，请先配对蓝牙设备",Toast.LENGTH_LONG).show();
		       		finish();
		       		return;
		       	}
		       	
		       	Spinner spinner = (Spinner)findViewById(R.id.spinner1);
		       	List<String>list = new ArrayList<String>();
		       	for (BluetoothDevice device:pairedDevices) {
		       		//myArrayAdapter.add(device.getName() + " " + device.getAddress());
		       		//list.add(device.getName() + " " + device.getAddress());
		       		list.add(device.getName()+" "+device.getAddress());
		       	}
		       	//??????????????
		       	ArrayAdapter<String> adapter = new ArrayAdapter<String>(getApplicationContext(),android.R.layout.simple_spinner_item,list);		       	
		       	adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		       	spinner.setAdapter(adapter);
		       	
		       	
		       	Button connectBtn = (Button)findViewById(R.id.connectBtn);
		       	connectBtn.setEnabled(true);
			}
		}
		);
		
		Button connectBtn = (Button)findViewById(R.id.connectBtn);
		connectBtn.setEnabled(false);
		connectBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				Spinner spinner = (Spinner)findViewById(R.id.spinner1);
				String[] ss=spinner.getSelectedItem().toString().split(" ");

				address =ss[ss.length-1];

				Log.d("@@TESTADDRESSS",address);
				try {
				  device = mBluetoothAdapter.getRemoteDevice(address);
				  clientSocket = device.createRfcommSocketToServiceRecord(uuid);
			 	  clientSocket.connect();
			 	  
			 	  mmOutStream = clientSocket.getOutputStream();
			 	  mmInStream  = new DataInputStream(new BufferedInputStream(clientSocket.getInputStream()));                     
			 	  Toast.makeText(getApplicationContext(), "连接成功", Toast.LENGTH_SHORT).show();

			 	  set_btn_status(true);
			 	  			 	  

			 	  blue_tooth_msg_thread = new bluetoothMsgThread(mmInStream,bluetoothMessageHandle);
  		 	      blue_tooth_msg_thread.start();

				}catch (Exception e) {
					set_btn_status(false);
					Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
					e.printStackTrace();
				}
			}
		});
		
		
		Button ledonBtn = (Button)findViewById(R.id.headBtn);
		ledonBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				 byte[] InBuffer = new byte[64];
			 	 byte buffer[] = "999".getBytes();
			 	 try {
			 	   mmOutStream.write(buffer);
			 	   //mmInStream.readFully(InBuffer, 0, 8);
			 	   //show_result(InBuffer,8);

				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		
		Button ledoffBtn = (Button)findViewById(R.id.retreatBtn);
		ledoffBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				 byte[] InBuffer = new byte[64];
			 	 byte buffer[] = "998".getBytes();
			 	 try { 
			 	   mmOutStream.write(buffer);
			 	   //mmInStream.readFully(InBuffer, 0, 8);
			 	   //show_result(InBuffer,8);

				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		
		Button jdqonBtn = (Button)findViewById(R.id.leftBtn);
		jdqonBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				 byte[] InBuffer = new byte[64];
			 	 byte buffer[] = "997".getBytes();
			 	 try {
			 	   mmOutStream.write(buffer);
			 	   //mmInStream.readFully(InBuffer, 0, 8);
			 	   //show_result(InBuffer,8);

				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		
		Button jdqoffBtn = (Button)findViewById(R.id.rightBtn);
		jdqoffBtn.setOnClickListener(new View.OnClickListener() { 						
			@Override
			public void onClick(View arg0) {
				 byte[] InBuffer = new byte[64];
			 	 byte buffer[] = "996".getBytes();
			 	 try { 
			 	  mmOutStream.write(buffer);
			 	  //mmInStream.readFully(InBuffer, 0, 8);
			 	  //show_result(InBuffer,8);

				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});


		Button speedBtn = (Button)findViewById(R.id.sendBtn);
		speedBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				if (TextUtils.isEmpty(editText.getText()))
				{
					Toast.makeText(getApplicationContext(), "输入框为空！", Toast.LENGTH_SHORT).show();
					return;

				}

				String s=editText.getText().toString();
				byte[] InBuffer = new byte[64];
				byte buffer[] = s.getBytes();
				try {
					mmOutStream.write(buffer);
					//mmInStream.readFully(InBuffer, 0, 8);
					//show_result(InBuffer,8);

				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		
		
	}
	
	@SuppressLint("HandlerLeak")
	Handler bluetoothMessageHandle = new Handler() {            //??????? handler ????
		public void handleMessage(Message msg) {
		  if (msg.what == 0x1234) {
			show_result((byte [])msg.obj,msg.arg1);
		  }
		}
	};
	
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private void initPression() {
		//???????
		// Here, thisActivity is the current activity
		if (ContextCompat.checkSelfPermission(this,
				Manifest.permission.ACCESS_COARSE_LOCATION)
				!= PackageManager.PERMISSION_GRANTED) {
			ActivityCompat.requestPermissions(this, new String[]{
					Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION
			}, 1);
		}
	}

}
