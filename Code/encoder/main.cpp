#include <iostream>

using namespace std;
int Altitude,Throttle;
bool AutoPilot,Grabber;
char str[8];

void encode(){
        str[0] = (char)('0' + Altitude/100);
        str[1] = (char)('0' + Altitude/10 - Altitude/100*10);
        str[2] = (char)('0' + Altitude%10);
        str[3] = (char)('0' + Throttle/100);
        str[4] = (char)('0' + Throttle/10 - Throttle/100*10);
        str[5] = (char)('0' + Throttle%10);
        str[6] = (char)('0' + AutoPilot);
        str[7] = (char)('0' + Grabber);
        cout<<str;
}
int main()
{
    cout << "altitude: "; cin>>Altitude;
    cout << "throttle: "; cin>>Throttle;
    cout << "autopilot: "; cin>>AutoPilot;
    cout << "servo: "; cin>>Grabber;
    encode();

    return 0;

}

