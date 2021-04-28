package iot.unipi.it.model;

public class Resource {
	
	private String name;
    private String path;
    
    public Resource(String name, String path) {
    	this.path = path;
	}
    
    public String getName() {
		return name;
	}
    
    public String getPath() {
		return path;
	}
    
}