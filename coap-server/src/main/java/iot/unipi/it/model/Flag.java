package iot.unipi.it.model;

public class Flag extends Resource{
	
	private String flagColor;

	public Flag(String name, String path, String flagColor) {
		super(name, path);
		this.flagColor = flagColor;
	}
	
	public String getFlagColor() {
		return flagColor;
	}
	
}