package iot.unipi.it;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.coap.MediaTypeRegistry;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;
import org.json.simple.parser.ParseException;

import iot.unipi.it.model.Flag;
import iot.unipi.it.model.Resource;
import iot.unipi.it.model.TrackLimit;

public class Application {
	
	
	private static Application sharedInstance = new Application();
	
	public static Application getSharedInstance() {
		return sharedInstance;
	}
	
	private Map<String, TrackLimit> tracklimitsMap = new HashMap<String, TrackLimit>();
	private Map<String, Flag> flagsMap = new HashMap<String, Flag>();
	private Map<String,	ObservableCoapClient> observableClients = new HashMap<String, ObservableCoapClient>();
	
	private boolean isObservingMode = false;
	
	public Map<String, TrackLimit> getTracklimitsMap() {
		return tracklimitsMap;
	}
	
	public Map<String, Flag> getFlagsMap() {
		return flagsMap;
	}
	
	public boolean isObservingMode() {
		return isObservingMode;
	}
	
	public Map<String, ObservableCoapClient> getObservableClients() {
		return observableClients;
	}
	
	public void setObservingMode(boolean isObservingMode) {
		this.isObservingMode = isObservingMode;
	}

	public static void main(String[] args) {
		Application.getSharedInstance().start();
	}
	
	private void start() {
		System.out.println("Welcome");
		runServer();
		System.out.println("Server started");
		System.out.println("Registration...");
		
		showMenu();
		while(true) {
			int cmd = getCommand();
			switch(cmd) {
				case 1:
					showAvailableTracklimits();
					showMenu();
					break;
				case 2:
					showAvailableFlags();
					showMenu();
					break;
				case 3:
					setResourceSector();
					showMenu();
					break;
				case 4:
					checkFlagColor();
					showMenu();
					break;
				case 5:
					changeFlagColor();
					showMenu();
					break;
				case 6:
					observingResource();
					break;
				case 0:
					System.out.println("Leaving the Application...");
					System.out.println("Bye.");
					System.exit(0);
					break;
				default:
					showMenu();
					break;
			}
		}
	}
	
	private void runServer() {
			
		new Thread() {
			public void run() {
				Server s = new Server();
				s.startServer();
			}
		}.start();
	}
	
	
	// Menu stuff
	
	private void showAvailableTracklimits() {
		if (tracklimitsMap.size() == 0) {
			System.out.println("\nNo Available Track Limit Sensors\n");
			return;
		}
		System.out.println("\nAvailable Track Limit Sensors: \n");
		for(Map.Entry<String,TrackLimit> entry : tracklimitsMap.entrySet()) {
			System.out.println(entry.getValue().toString());
		}
	}
	
	private void showAvailableFlags() {
		if (flagsMap.size() == 0) {
			System.out.println("\nNo Available Flags\n");
			return;
		}
		System.out.println("\nAvailable Track Flags: \n");
		for(Map.Entry<String,Flag> entry : flagsMap.entrySet()) {
			System.out.println(entry.getValue().toString());
		}
	}
	
	private void setResourceSector() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		try {
			System.out.println("\nInsert the resource name: ");
			System.out.print(">> ");
			String name;
			name = input.readLine();
			Resource res;
			if(name.contains("flag")) {
				res = this.flagsMap.get(name);
			} else {
				res = this.tracklimitsMap.get(name);
			}
			if(res == null) {
				System.err.println("\nNo resource with this name exists");
				return;
			}
			System.out.println("Insert the resource sector: ");
			System.out.print(">> ");
			int sector = getCommand();
			res.setSector(sector);
		} catch (Exception e) {
			System.err.println("Error: " + e.getMessage());
		}
	}
	
	private void checkFlagColor() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Insert the flag name: ");
		System.out.print(">> ");
		String name;
		try {
			name = input.readLine();
			Flag flag = this.flagsMap.get(name);
			if(flag == null) {
				System.err.println("No flag with this name exists");
				return;
			}
			CoapClient flagClient = new CoapClient(flag.getCoapURI());
			CoapResponse response = flagClient.get();
			String code = response.getCode().toString();
			if(!code.startsWith("2")) {
				System.err.println("Error:"+code);
				return;
			}
			System.out.println("The color of the flag is " + this.getFlagColor(response.getResponseText()));
		} catch (IOException e) {
			System.err.println("Error: " + e.getMessage());
		}
	}
	
	private void changeFlagColor() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("\nInsert the flag name: ");
		System.out.print(">> ");
		String name;
		try {
			name = input.readLine();
			Flag flag = this.flagsMap.get(name);
			if(flag == null) {
				System.err.println("No flag with this name exists");
				return;
			}
			System.out.println("Insert the flag color: ");
			System.out.print(">> ");
			String flagColor = input.readLine();
			if (!this.isValidFlag(flagColor)){
				System.out.println("Error: the flag color is not valid.");
				return;
			}
			String postBody = "flag=" + flagColor;
			if(flagColor.equalsIgnoreCase("yellow")) {
				System.out.println("Insert the seconds: ");
				System.out.print(">> ");
				int seconds = getCommand();
				if (seconds != -1) {
					postBody += "&seconds=" + seconds;
				} else {
					postBody += "&seconds=10";
				}
			} else {
				postBody += "&seconds=10";
			}
			System.out.println("Post body: " + postBody);
			CoapClient flagClient = new CoapClient(flag.getCoapURI());
			CoapResponse response = flagClient.post(postBody, MediaTypeRegistry.TEXT_PLAIN);
			String code = response.getCode().toString();
			if(!code.startsWith("2")) {
				System.err.println("Error: "+ code);
				return;
			}
		} catch (IOException e) {
			System.err.println("Error: " + e.getMessage());
		}
	}
	
	public void setDangerDefaultFlag(TrackLimit source) {
		String postBody = "flag=yellow&seconds=10";
		List<Flag> flags = new ArrayList<Flag>();
		for (Flag flag : this.flagsMap.values()) {
			if (flag.getSector() == source.getSector()) {
				flags.add(flag);
			}
		}
		for (Flag flag2 : flags) {
			CoapClient flagClient = new CoapClient(flag2.getCoapURI());
			CoapResponse response = flagClient.post(postBody, MediaTypeRegistry.TEXT_PLAIN);
			String code = response.getCode().toString();
			if(!code.startsWith("2")) {
				System.err.println("Error: "+ code);
				return;
			}
		}
		System.out.println("Yellow flag set on sector: " + source.getSector());
	}
	
	private int getCommand() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		String line;
		int intCommand;
		try {
			line = input.readLine();
			intCommand = Integer.parseInt(line);
			return intCommand;
		} catch (IOException e) {
			return -1;
		} catch (NumberFormatException n) {
			return -1;
		}
	}
	
	public void showMenu() {
		System.out.print("\nPlease, insert a command: \n"
				+ "1 - Show Registered Track Limit Sensors.\n"
				+ "2 - Show Registered Flags\n"
				+ "3 - Set Resource Sector\n"
				+ "4 - Check Flag Color\n"
				+ "5 - Set flag color\n"
				+ "6 - Start Observing\n"
				+ ""
				+ "0 - Exit.\n"
				+ "\n"
				+ ">> ");
	}
	
	private boolean isValidFlag(String flag) {
		if(flag.equalsIgnoreCase("red")) {
			return true;
		}
		if(flag.equalsIgnoreCase("yellow")) {
			return true;
		}
		if(flag.equalsIgnoreCase("green")) {
			return true;
		}
		return false;
	}
	
	private void observingResource() {
		System.out.println("\n\nObservable Resources\n");
		for (ObservableCoapClient coapClient : this.getObservableClients().values()) {
			System.out.println(coapClient.getRes());
		}
		
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("\n-------- YOU ARE IN OBSERVING MODE --------\n");
		for (ObservableCoapClient observable : this.observableClients.values()) {
			observable.startObserving();
		}
		System.out.println("Digit q if you want to quit this mode");
		this.isObservingMode = true;
		String c = "";
		while(true) {
			try {
				c = br.readLine();
				if(c.equals("q")) {
					for (ObservableCoapClient observable : this.observableClients.values()) {
						observable.stopObserving();
					}
					return;
				}
			} catch (IOException e) {
				System.out.println("Observing mode failed: " + e.getLocalizedMessage());
			}
		}
	}
	
	private String getFlagColor(String message) {
		try {
			JSONObject obj = (JSONObject) JSONValue.parseWithException(message);
			if (obj.containsKey("actual_flag")) {
				int color = Integer.parseInt(obj.get("actual_flag").toString());
				System.out.println("Flag color: " + color);
				switch (color) {
					case 0: return "green";
					case 1: return "yellow";
					case 2: return "red";
					default: return "Uncorrect value";
				}
			}
			return "Uncorrect value";
		} catch (ParseException e) {
			e.printStackTrace();
			return "Uncorrect value";
		}
		
	}

}