/*primitive encoding*/
/*
	ALIGN -> allow phy layer to re-adjust its internal operation
	binary : 11011 011	01010 010	01010 010	11100 101

	CONT -> repeat the perviously received primitive as long as there is no another primitive received
	binary : 11001 100	11001 100	01010 101	11100 011

	DMAT -> terminate DMA data transmission by the transmitter
	binary : 10110 001	10110 001	10101 101	11100 011

	EOF -> marks the end of frame, the previous non-primitive Dword is the CRC of the frame
	binary : 10101 110	10101 110	10101 101	11100 011

	HOLD -> hold transmitting / receiving data
	binary : 10101 110	10101 110	01010 101	11100 011

	HOLDA -> this primitive is sent while HOLD is received "HOLD Acknowledgement"
	binary : 10101 100	10101 100	01010 101	11100 011

	PMACK -> sent in response to "PMREQ_S" or "PMREQ_P" if a receiving node is prepared to enter a power mode state
	binary : 10101 100	10101 100	10101 100	11100 011

	PMNAK -> Sent in response to a "PMREQ_S" or "PMREQ_P" if a receiving node is not prepared to enter a power mode state
	or if power management is not supported
	binary : 10101 111	10101 111	10101 100	11100 011

	PMREQ_P -> This primitive is sent continuously until "PMACK" or "PMNAK" is received
	binary : 10111 000	10111 000	10101 101	11100 011

	PMREQ_S -> This primitive is sent continuously until "PMACK" or "PMNAK" is received
	binary : 10101 011	10101 011	10101 100	11100 011

	R_ERR -> current node detect reception error
	binary : 10110 010	10110 010	10101 101	11100 011

	R_IP -> current node is receiving payload
	binary : 10101 010	10101 010	10101 101	11100 011

	R_OK -> current node detect no error in received payload
	binary : 10101 001	10101 001	10101 101	11100 011

	R_RDY -> current node is ready to receive payload
	binary : 01010 010	01010 010	10101 100	11100 011

	SOF -> start of frame. payload and CRC follow until EOF
	binary : 10111 001	10111 001	10101 101	11100 011

	SYNC -> synchronizing primitive
	binary : 10101 101	10101 101	10101 100	11100 011

	WTRM -> (wait for frame termination) after transmission of "EOF", the transmitter send "WTRM"
	binary : 11000 010	11000 010	10101 101	11100 011

	X_RDY -> current node has payload ready for transmission
	binary : 10111 010	10111 010	10101 101	11100 011
*/

#include <systemc.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define ALIGN 0xDB5252E5 //11011011 01010010 01010010 11100101
#define EOF_ 0xAEAEADE3 //10101110 10101110 10101101 11100011
#define HOLD 0xAEAE55E3 //10101110 10101110 01010101 11100011
#define HOLDA 0xACAC55E3 //10101100 10101100 01010101 11100011
#define R_ERR 0xB2B2ADE3 //10110010 10110010 10101101 11100011
#define R_IP 0xAAAAADE3 //10101010 10101010 10101101 11100011
#define R_OK 0xA9A9ADE3 //10101001 10101001 10101101 11100011
#define R_RDY 0x5252ACE3 //01010010 01010010 10101100 11100011
#define SOF 0xB9ADADE3 //10111001 10111001 10101101 11100011
#define SYNC 0xADADACE3 //10101101 10101101 10101100 11100011
#define WTRM 0xC2C2ADE3 //11000010 11000010 10101101 11100011
#define X_RDY 0xBABAADE3 //10111010 10111010 10101101 11100011


SC_MODULE (prim_gen)
{
	sc_in < sc_uint<4> > prim_sel;
	sc_out < sc_uint<32> > prim;
	sc_event e;
	void func () {
		sc_uint<4> tmp;
		tmp = prim_sel.read();

		switch (tmp) {
			case 0x0:
				prim.write(ALIGN);
				break;
			case 0x1:
				prim.write(EOF_);
				break;
			case 0x2:
				prim.write(HOLD);
				break;
			case 0x3:
				prim.write(HOLDA);
				break;
			case 0x4:
				prim.write(R_ERR);
				break;
			case 0x6:
				prim.write(R_IP);
				break;
			case 0x5:
				prim.write(R_OK);
				break;
			case 0x7:
				prim.write(R_RDY);
				break;
			case 0xB:
				prim.write(SOF);
				break;
			case 0x9:
				prim.write(SYNC);
				break;
			case 0x8:
				prim.write(WTRM);
				break;
			case 0xC:
				prim.write(X_RDY);
				break;
			default:
				prim.write(ALIGN);
				break;
			}
	}

	SC_CTOR (prim_gen)
	{
		SC_METHOD (func);
		sensitive << prim_sel << e;
	}
};

int sc_main(int argc, char* argv[]) {
	//declaring variables
	sc_signal < sc_uint<4> > prim_sel;
	sc_signal < sc_uint<32> > prim;

	//declaring object from prim_gen
	prim_gen prim1("prim_gen");

	//connecting ports
	prim1.prim_sel(prim_sel);
	prim1.prim(prim);

	//starting event "e" + starting simulation via sc_start
	prim1.e;
	prim_sel.write(0x4);
	sc_start(1,SC_NS);
	prim1.e.notify(SC_ZERO_TIME);
	cout<<"output is "<< hex <<prim.read();
	cout << endl;
	return 0;
}