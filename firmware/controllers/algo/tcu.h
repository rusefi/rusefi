

class TCU {
public:
	gear_e currentGear = NEUTRAL;
	gear_e desiredGear = NEUTRAL;
private:
	efitimems_t timeLastShift = 0;
};
