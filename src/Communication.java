public class Communication {
    CallbackInterface callbackInterface;

    public Communication(CallbackInterface callbackInterface) {
        this.callbackInterface = callbackInterface;

        // you can use
        callbackInterface.requestMembership("someID");
        callbackInterface.cancelMembership("someID");
    }
}
