package iot.unipi.it;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapObserveRelation;

import iot.unipi.it.model.Resource;

public class ObserveClient extends CoapClient {
	
	private Resource res;
	CoapObserveRelation coapObserveRelation;
	
	public ObserveClient(Resource res) {
		super(res.getCoapURI());
		this.res= res;
	}
	
	public Resource getRes() {
		return res;
	}
	
	public void startObserving() {
		
	}
}
