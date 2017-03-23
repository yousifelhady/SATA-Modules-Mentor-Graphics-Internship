/*32b input - 40b output*/

#include <systemc.h>
#include <iostream>
using namespace std;

SC_MODULE (encoder)
{
	sc_event e;
	sc_in< sc_bv<32> > inp;
	sc_out< sc_bv<40> > result;
	bool rdin, rdout_tmp, current_rd, encoder_err, a7p7_cond;	

	void func ()
	{
		sc_uint<32> tmp = inp.read();
		input_decomposer(tmp);
	}

	void input_decomposer (sc_uint<32> input_word)
	{
		sc_uint<32> Dword = input_word;
		sc_uint<8> byte0 = Dword.range(7, 0);
		sc_uint<8> byte1 = Dword.range(15, 8);
		sc_uint<8> byte2 = Dword.range(23, 16);
		sc_uint<8> byte3 = Dword.range(31, 24);
		sc_bv<40> output;
		output.range(9, 0) = byte_decomposer(byte0);
		output.range(19, 10) = byte_decomposer(byte1);
		output.range(29, 20) = byte_decomposer(byte2);
		output.range(39, 30) = byte_decomposer(byte3);
		result.write(output);
	}

	sc_bv<10> byte_decomposer (sc_uint<8> input_byte)
	{
		sc_uint<8> byte = input_byte;
		sc_uint<5> tmp1 = byte.range(4,0);
		sc_uint<3> tmp2 = byte.range(7,5);
		sc_bv<10> output3 = encode5to6(tmp1) | encode3to4(tmp2);
		a7p7_cond = ((current_rd == 1) & (output3.range(5, 4) == 0x0)) |
                     ((current_rd == 0) & (output3.range(5, 4) == 0x3));
		return output3;
	}
	
	sc_bv<10> encode5to6 (sc_uint<5> a)
	{
		sc_bv<10> output1;
		switch (a)
		{
		case 0x00:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x27 : 0x18;
			break;
		case 0x01:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x1D : 0x22;
			break;
		case 0x02:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x2D : 0x12;
			break;
		case 0x03:
			current_rd = rdin;
			output1.range(5, 0) = 0x31;
			break;
		case 0x04:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x35 : 0x0A;
			break;
		case 0x05:
			current_rd = rdin;
			output1.range(5, 0) = 0x29;
			break;
		case 0x06:
			current_rd = rdin;
			output1.range(5, 0) = 0x19;
			break;
		case 0x07:
			current_rd = rdin;
            output1.range(5, 0) = (rdin) ? 0x07 : 0x38;
			break;
		case 0x08:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x39 : 0x06;
			break;
		case 0x09:
			current_rd = rdin;
			output1.range(5, 0) = 0x25;
			//cout << output1 << endl;
			break;
		case 0x0A:
			current_rd = rdin;
			output1.range(5, 0) = 0x15;
			//cout << output1 << endl;
			break;
		case 0x0B:
			current_rd = rdin;
			output1.range(5, 0) = 0x34;
			break;
		case 0x0C:
			current_rd = rdin;
			output1.range(5, 0) = 0x0D;
			break;
		case 0x0D:
			current_rd = rdin;
			output1.range(5, 0) = 0x2C;
			//cout << output1 << endl;
			break;
		case 0x0E:
			current_rd = rdin;
			output1.range(5, 0) = 0x1C;
			//cout << output1 << endl;
			break;
		case 0x0F:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x17 : 0x28;
			break;
		case 0x10:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x1B : 0x24;
			break;
		case 0x11:
			current_rd = rdin;
			output1.range(5, 0) = 0x23;
			break;
		case 0x12:
			current_rd = rdin;
			output1.range(5, 0) = 0x13;
			//cout << output1 << endl;
			break;
		case 0x13:
			current_rd = rdin;
			output1.range(5, 0) = 0x32;
			break;
		case 0x14:
			current_rd = rdin;
			output1.range(5, 0) = 0x0B;
			break;
		case 0x15:
			current_rd = rdin;
			output1.range(5, 0) = 0x2A;
			//cout << output1 << endl;
			break;
		case 0x16:
			current_rd = rdin;
			output1.range(5, 0) = 0x1A;
			break;
		case 0x17:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x3A : 0x05;
			break;
		case 0x18:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x33 : 0x0C;
			break;
		case 0x19:
			current_rd = rdin;
			output1.range(5, 0) = 0x26;
			break;
		case 0x1A:
			current_rd = rdin;
			output1.range(5, 0) = 0x16;
			break;
		case 0x1B:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x36 : 0x09;
			break;
		case 0x1C:
			current_rd = rdin;
			output1.range(5, 0) = 0x0E;
			break;
		case 0x1D:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x2E : 0x11;
			break;
		case 0x1E:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x1E : 0x21;
			break;
		case 0x1F:
			current_rd = ~rdin;
            output1.range(5, 0) = (~rdin) ? 0x2B : 0x14;
			break;
		}
		/*
		switch (a)
		{
		casex 0x1C:
			current_rd = rdin;
			output1.range(5, 0) = (~rdin) ? 0x30 : 0x0F;
			break;
		default:
			current_rd = rdin;
			encoder_err = 1;
			break;
		}
		*/
		return output1;
	}

	sc_bv<10> encode3to4 (sc_uint<3> a)
	{
		sc_bv<10> output2;
		
		switch (a)
		{
		case 0x0:
			rdout_tmp = ~current_rd;
            output2.range(9, 6) = (~current_rd) ? 0xB : 0x4;
			break;
		case 0x1:
			rdout_tmp = current_rd;
			output2.range(9, 6) = 0x9;
			break;
		case 0x2:
			rdout_tmp = current_rd;
			output2.range(9, 6) = 0x5;
			//cout << output2 << endl;
			break;
		case 0x3:
			rdout_tmp = current_rd;
            output2.range(9, 6) = (~current_rd) ? 0xC : 0x3;
			break;
		case 0x4:
			rdout_tmp = ~current_rd;
            output2.range(9, 6) = (~current_rd) ? 0xD : 0x2;
			break;
		case 0x5:
			rdout_tmp = current_rd;
			output2.range(9, 6) = 0xA;
			//cout << output2 << endl;
			break;
		case 0x6:
			rdout_tmp = current_rd;
			output2.range(9, 6) = 0x6;
			//cout << output2 << endl;
			break;
		case 0x7:
			rdout_tmp = ~current_rd;
            if (a7p7_cond)
				output2.range(9, 6) = (~current_rd) ? 0xE : 0x1;
            else
				output2.range(9, 6) = (~current_rd) ? 0x7 : 0x8;
		default:
			rdout_tmp = current_rd;
			encoder_err = 1;
		}
		return output2;
	}

	SC_CTOR (encoder)
	{
		encoder_err = 0; //Initially No Encoding Error
		SC_METHOD (func);
		sensitive << inp << e;
	}
};

int sc_main(int argc, char* argv[])
{
	sc_signal < sc_bv<32> > input;
	sc_signal < sc_bv<40> > result;
	encoder encoder("encoder");

	encoder.inp(input);
	encoder.result(result);
	encoder.e;
	
	//input.write(0xAD4A31D5); //D13.5 D10.2 D17.1 D21.6
	input.write(0x61840F4A);	//D1.3 D4.4 D15.0 D10.2
	sc_start(1, SC_NS);
	encoder.e.notify(SC_ZERO_TIME);

	cout <<"Input is: " << input.read() << endl;
	cout <<"Output is: " << result << endl;
	cout <<"Output in hexa format: " << hex << result;

	cout << endl;
	return 0;
}