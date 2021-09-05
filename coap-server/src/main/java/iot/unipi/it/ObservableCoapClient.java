package iot.unipi.it;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapHandler;
import org.eclipse.californium.core.CoapObserveRelation;
import org.eclipse.californium.core.CoapResponse;

import iot.unipi.it.model.Resource;
import iot.unipi.it.model.TrackLimit;

public class ObservableCoapClient extends CoapClient {
	
	private TrackLimit res;
	CoapObserveRelation coapObserveRelation;
	
	public ObservableCoapClient(TrackLimit res) {
		super(res.getCoapURI());
		this.res= res;
	}
	
	public Resource getRes() {
		return res;
	}
	
	public void startObserving() {
		this.coapObserveRelation = this.observe(new CoapHandler() {
			
			public void onLoad(CoapResponse response) {
				String content = response.getResponseText(); 
				System.out.println("Res: " + res.getName() + " --> " + content);
			}
			
			public void onError() {
				System.out.println("--------ERROR DURING OBSERVING RESOURCE--------");
			}
		});
		System.out.println("Observing Mode ON for tracklimit: " + res.getName());
	}
	
	public void stopObserving() {
		this.coapObserveRelation.reactiveCancel();
		System.out.println("Observing Mode OFF for tracklimit: " + res.getName());
	}
	
	
}
