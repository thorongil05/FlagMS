package iot.unipi.it.model;

public class Flag extends Resource{
	
	private String flagColor;
	private int seconds;

	public Flag(String name, String path, String ip, boolean observable, String flagColor, int seconds) {
		super(name, path, ip, observable);
		this.flagColor = flagColor;
		this.seconds = seconds;
	}
	
	public String getFlagColor() {
		return flagColor;
	}
	
	public int getSeconds() {
		return seconds;
	}
}