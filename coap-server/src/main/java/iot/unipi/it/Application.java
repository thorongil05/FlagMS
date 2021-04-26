package iot.unipi.it;

public class Application {

	public static void main(String[] args) {
		
		System.out.println("Welcome");
		
		runServer();
		
	}
	
	public static void runServer() {
			
		new Thread() {
			public void run() {
				Server s = new Server();
				s.startServer();
			}
		}.start();
			
	}

}