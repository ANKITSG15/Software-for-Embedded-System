package com.example.myapplication2;

public class FireDB {

    private String hours;
    private String minute;
    private String seconds;
    private String clicked;

    public FireDB(String hours, String minute, String seconds, String clicked) {
        this.hours = hours;
        this.minute = minute;
        this.seconds = seconds;
        this.clicked = clicked;
    }

    public String getHours() {
        return hours;
    }

    public void setHours(String hours) {
        this.hours = hours;
    }

    public String getMinute() {
        return minute;
    }

    public void setMinute(String minute) {
        this.minute = minute;
    }

    public String getSeconds() {
        return seconds;
    }

    public void setSeconds(String seconds) {
        this.seconds = seconds;
    }

    public String getClicked() {
        return clicked;
    }

    public void setClicked(String clicked) {
        this.clicked = clicked;
    }
}
