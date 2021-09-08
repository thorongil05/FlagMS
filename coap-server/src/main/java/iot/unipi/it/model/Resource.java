package iot.unipi.it.model;

public class Resource {
	
	private String name;
    private String path;
    private boolean observable;
    private String ip;
    private int sector = 1;
    
    public Resource(String name, String path, String ip, boolean observable) {
    	this.name = name;
    	this.path = path;
    	this.ip = ip;
    	this.observable = observable;
	}
    
    public String getName() {
		return name;
	}
    
    public int getSector() {
		return sector;
	}
    
    public String getPath() {
		return path;
	}
    
    public boolean isObservable() {
    	return this.observable;
    }
    
    public String getCoapURI(){ 
    	return "coap://[" + this.ip+"]:5683"+ this.path;
    }
    
    public void setSector(int sector) {
		this.sector = sector;
	}
    
    @Override
    public String toString() {
    	StringBuilder builder = new StringBuilder();
    	builder.append(this.name);
    	builder.append(" - ");
    	builder.append("sector " + this.sector);
    	builder.append(" - ");
    	builder.append(this.path);
    	builder.append(" - ");
    	builder.append(" observable: ");
    	builder.append(this.observable);
    	builder.append(" - ");
    	builder.append(" ip: " + this.ip);
    	return builder.toString();
    }
    
}