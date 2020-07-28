package com.example.myapplication2;

import androidx.appcompat.app.AppCompatActivity;

import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.Calendar;


public class MainActivity extends AppCompatActivity {
    private AppDB appDb;;
    TextView mTextView;
    private FireDB fireDb;
    Button mButtonRec,mButtonShow;
    int mButtonClickCount = 0;
    Calendar now = null;
    FirebaseDatabase database;
    DatabaseReference myRef;

    public static final int COL_SID = 0;
    public static final int COL_HOURS = 1;
    public static final int COL_MINUTES = 2;
    public static final int COL_SECONDS = 3;
    public static final int COL_LIGHT = 4;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);
        // Instantiate textView and button objects
        mTextView = (TextView) findViewById(R.id.text_view);
        mButtonRec = (Button) findViewById(R.id.recd_button);
        mButtonShow = (Button) findViewById(R.id.show_button);
        // Instantiate the App DB
            appDb = new AppDB(this);
        // Open the AppDB;
            appDb.open();
        // Called when click on Record Button
        mButtonRec.setOnClickListener(new View.OnClickListener()
            {
                @Override
                 public void onClick(View v) {

                     try {
                         now = Calendar.getInstance();
                         int date = now.get(Calendar.DAY_OF_MONTH);
                         int hour = now.get(Calendar.HOUR_OF_DAY);
                         int min = now.get(Calendar.MINUTE);
                         int sec = now.get(Calendar.SECOND);

                         mButtonClickCount++;
                         mTextView.setText("");
                         Log.w("MainActivity", "Now sending the values on SQLite DB");
                         boolean flag = appDb.insertData(date, hour, min, sec, mButtonClickCount);

                         if(!flag)
                             Log.w("MainActivity", "Data is  not inserted into SQLite DB");

                         Log.w("MainActivity", "Now sending the values on Firebase DB");
                         database = FirebaseDatabase.getInstance();
                         myRef = database.getReference("Clicked");
                         fireDb = new FireDB(Integer.toString(hour), Integer.toString(min),
                                 Integer.toString(sec), Integer.toString(mButtonClickCount));
                         myRef.setValue(fireDb);
                         }catch(Exception e)
                            {
                                 Log.e("MainActivity",
                                 "Error in inserting data in DB " + e.getMessage());

                                mTextView.setText("Some Exception is raised either due to insertion in SQLite or" +
                                             "Firebase, Please check the logs");
                            }
                 }
            });

        mButtonShow.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v) {
                String displayText = "";
                // If we do at the beginning, it will show that date and time only

                // Query the data for current date
                try {
                    Cursor cursor = appDb.getAllRows();
                    cursor.moveToFirst();
                    if(cursor !=null && cursor.getCount()<1)
                    {
                        Log.e("MainActivity",
                                "Cursor is empty check size: " + cursor.getCount());

                        mTextView.setText("No Data is present to show.");
                    }
                     else {
                        do {
                            int hrs = cursor.getInt(COL_HOURS);
                            int mnts = cursor.getInt(COL_MINUTES);
                            int secs = cursor.getInt(COL_SECONDS);
                            int light = cursor.getInt(COL_LIGHT);

                            displayText = displayText + String.format("%02d",hrs) + ":" +
                                    String.format("%02d",mnts) + ":" +
                                    String.format("%02d",secs) + " - " +
                                    String.format("%02d",light) + "\n";
                        } while (cursor.moveToNext());

                        mTextView.setText(displayText);
                    }
                }catch(Exception e)
                {
                    Log.e("MainActivity",
                            "Error in cursor " + e.getMessage());

                    mTextView.setText("Some Exception is raised, Please check the logs");
                }
            }
        });


    }
}