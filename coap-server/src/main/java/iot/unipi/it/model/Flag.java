package iot.unipi.it.model;

public class Flag extends Resource{
	
	private String flagColor;
	private int seconds;

	public Flag(String name, String path, String ip, boolean observable) {
		super(name, path, ip, observable);
	}
	
	public String getFlagColor() {
		return flagColor;
	}
	
	public int getSeconds() {
		return seconds;
	}
	
	public void setFlagColor(String flagColor) {
		this.flagColor = flagColor;
	}
	
	public void setSeconds(int seconds) {
		this.seconds = seconds;
	}
}