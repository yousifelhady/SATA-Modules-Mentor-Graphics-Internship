#include <systemc.h>
#include <iostream>
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


SC_MODULE (prim_decoder)
{
	sc_out < sc_uint<4> > prim_to_ll;
	sc_in < sc_bv<32> > prim_in;
	sc_out < sc_uint<2> > error_encoding;
	sc_event e;
	void func () {
		sc_uint<32> tmp;
		tmp = prim_in.read();
		error_encoding.write(0);
		switch (tmp) {
			case ALIGN:
				prim_to_ll.write(0x0);
				break;
			case EOF_:
				prim_to_ll.write(0x1);
				break;
			case HOLD:
				prim_to_ll.write(0x2);
				break;
			case HOLDA:
				prim_to_ll.write(0x3);
				break;
			case R_ERR:
				prim_to_ll.write(0x4);
				break;
			case R_IP:
				prim_to_ll.write(0x6);
				break;
			case R_OK:
				prim_to_ll.write(0x5);
				break;
			case R_RDY:
				prim_to_ll.write(0x7);
				break;
			case SOF:
				prim_to_ll.write(0xB);
				break;
			case SYNC:
				prim_to_ll.write(0x9);
				break;
			case WTRM:
				prim_to_ll.write(0x8);
				break;
			case X_RDY:
				prim_to_ll.write(0xC);
				break;
			default:
				cout <<"Error in primitive detection"<< endl;
				error_encoding.write(1);
			}
	}

	SC_CTOR (prim_decoder)
	{
		SC_METHOD (func);
		sensitive << prim_in << e;
	}
};

int sc_main(int argc, char* argv[]) {
	//declaring variables
	sc_signal < sc_uint<4> > prim_to_ll;
	sc_signal < sc_bv<32> > prim_in;
	sc_signal < sc_uint<2> > error_encoding;

	//declaring object from prim_decoder
	prim_decoder primDecoder("prim_decoder");

	//connecting ports
	primDecoder.prim_to_ll(prim_to_ll);
	primDecoder.prim_in(prim_in);
	primDecoder.error_encoding(error_encoding);

	//starting event "e" + starting simulation via sc_start
	primDecoder.e;
	prim_in.write(0xBABAADE3); //output -> C
	//prim_in.write(0xFFFFFFFF); //Error test
	sc_start(1,SC_NS);
	primDecoder.e.notify(SC_ZERO_TIME);
	cout<<"output is "<< hex <<prim_to_ll.read();
	cout << endl;
	return 0;
}