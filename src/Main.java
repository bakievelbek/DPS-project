import java.util.ArrayList;

public class Main implements CallbackInterface {
    private Communication communication;
    private Sensors sensors;
    private VehicleControl vehicleControl;
    private VehicleState vehicleState;

    private String id;
    private String memberOf = "";
    private final ArrayList<String> memberRequests = new ArrayList<>();
    private final ArrayList<String> members = new ArrayList<>();

    public static void main(String[] args) {
        System.out.println("Truck Platoon Management System");
        if (args.length != 1) {
            System.out.println("Usage: dandp id");  // in IntelliJ click Main - edit config - proj arguments
            System.exit(1);
        }

        String id = args[0];
        System.out.println("Welcome " + id);
        Main main = new Main(id);
    }

    public Main(String id) {
        this.id = id;

        communication = new Communication(this);
        sensors = new Sensors();
        vehicleControl = new VehicleControl();
        vehicleState = new VehicleState();
    }

    public void requestMembership(String id) {
        if (!memberRequests.contains(id)) {
            memberRequests.add(id);
        }
    }
    public void cancelMembership(String id) {
        memberRequests.remove(id);
        members.remove(id);
    }
    private void acceptMember(String id) {
        memberRequests.remove(id);
        members.add(id);
    }
}
