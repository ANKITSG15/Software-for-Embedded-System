package com.example.myapplication2;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;



public class AppDBHelper extends SQLiteOpenHelper {


    public static final String TABLE_NAME = "mytable";
    public static final String COLUMN_SID = "sid";
    public static final String COLUMN_HOURS = "hours";
    public static final String COLUMN_MINUTES = "minutes";
    public static final String COLUMN_SECONDS = "seconds";
    public static final String COLUMN_LIGHT= "light";


    public AppDBHelper( Context context,String DBNAME,SQLiteDatabase.CursorFactory factory,int version) {
        super(context, DBNAME, factory,version);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {

         db.execSQL("create table "+TABLE_NAME+" ("+COLUMN_SID+" integer primary key autoincrement," +
                        ""+COLUMN_HOURS+" integer,"+COLUMN_MINUTES+" integer,"+COLUMN_SECONDS+" " +
                        "integer,"+COLUMN_LIGHT+" integer)");

         Log.w("AppDBHelper", "Database table " + TABLE_NAME + " was created.");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

        Log.w("AppDBHelper", "Upgrading from version" +
                oldVersion + "to" + newVersion +
                ", which will destroy the old data");

        //Drop the old table
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
        //Create a new one
        onCreate(db);

    }


}
