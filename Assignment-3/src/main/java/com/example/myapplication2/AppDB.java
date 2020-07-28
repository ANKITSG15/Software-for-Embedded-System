package com.example.myapplication2;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.util.Log;

public class AppDB {

    private SQLiteDatabase appDB;	//App Database
    private AppDBHelper appDBHelper;	//App Database Helper
    private final Context context;

    public static final String DATABASE_NAME = "MyDBName.db";
    public static final String TABLE_NAME = "mytable";
    public static final String COLUMN_SID = "sid";
    public static final String COLUMN_HOURS = "hours";
    public static final String COLUMN_MINUTES = "minutes";
    public static final String COLUMN_SECONDS = "seconds";
    public static final String COLUMN_LIGHT= "light";

    public AppDB(Context _context)
    {
        context = _context;

        // Instantiate the Database Helper
        appDBHelper = new AppDBHelper(context, DATABASE_NAME, null, 1);

        Log.w("AppDB", "AppDBHelper was instantiated");
    }

    public boolean open() throws SQLiteException
    {
        try {
            //Open the database for writing
            appDB = appDBHelper.getWritableDatabase();

            Log.w("AppDB", "AppDB:Database opened for writing");

            return true;
        } catch(SQLiteException ex)
        {
            //Open the database for reading, if exception occurs
            appDB = appDBHelper.getReadableDatabase();

            Log.w("AppDB", "AppDB:Database opened for reading only");

            return false;
        }
    }

    public void close()
    {
        appDB.close();

        Log.w("AppDB", "AppDB:Database closed");

    }

    //Inserting values into the table
    public boolean insertData(int date,int hrs,int min,int sec,int clicked){

        ContentValues contentValues = new ContentValues();
        contentValues.put(COLUMN_HOURS,hrs);
        contentValues.put(COLUMN_MINUTES,min);
        contentValues.put(COLUMN_SECONDS,sec);
        contentValues.put(COLUMN_LIGHT,clicked);
        long result = appDB.insert(TABLE_NAME,null,contentValues);
        if(result == -1) {
            Log.e("AppDB",
                    "Insert to database table " + TABLE_NAME + ":" + hrs + "." + min + " failed.");
            return false;
        }
        return true;
    }


    // Fetching the Values row by row from Table
    public Cursor getAllRows()
    {
        return appDB.query(	TABLE_NAME,
                new String[] {COLUMN_SID, COLUMN_HOURS, COLUMN_MINUTES, COLUMN_SECONDS, COLUMN_LIGHT},
                null, null, null, null, null);
    }



}
