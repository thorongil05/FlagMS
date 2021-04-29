package iot.unipi.it.model;

public class Resource {
	
	private String name;
    private String path;
    private boolean observable;
    
    public Resource(String name, String path, boolean observable) {
    	this.name = name;
    	this.path = path;
    	this.observable = observable;
	}
    
    public String getName() {
		return name;
	}
    
    public String getPath() {
		return path;
	}
    
    public boolean isObservable() {
    	return this.observable;
    }
    
}