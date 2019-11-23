#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myhead.h"

_UL ftoid(double);
void getbinary(_UL , int *);
void printbinary(int *);
void fprintbinary(FILE *, int *);

void data_gen(double start, double end, int counts) {
	FILE *data;
	double inputData;
	double temp;
	_UL temp_ul;
	int i, j, binary[64];

	printf("%d numbers generated in [%e, %e]\n", counts, start, end);
	data = fopen("data.txt", "w");
	srand(time(NULL));
	for (i = 0; i < counts; i++) {
		inputData=(double)rand() / ((double)RAND_MAX + 1) * (end - start) + (start);
		temp_ul = ftoid(inputData);
		getbinary(temp_ul, binary);
		//printbinary(binary);
		fprintbinary(data, binary);
	}

	fclose(data);
}


int main(int argc, char *argv[]) {
	double start, end;
	DL s, e;

	s.l = 0x3e40000000000000;
	start = s.d;
	end = 6.1359231515425649188723503579677790706975964831545e-3; // pi/512
	end = 1.2271846303085129837744700715935558141395192966309e-2; // pi/256
	end = 2.4543692606170259675489401431871116282790385932618e-2; // pi/128
	end = 4.9087385212340519350978802863742232565580771865236e-2; // pi/64
	//end = 9.817477042468103870195760572748446513116154373047e-2; // pi/32
	//end = 0.19634954084936207740391521145496893026232308746094; // pi/16
	start = -0.78539816339744830961566084581987572104929234984377; // -pi/4
	end = 0.78539816339744830961566084581987572104929234984377; // pi/4
	start = -1.57079632679489661923132169163975144209858469968754; // -pi/2
	end = 1.57079632679489661923132169163975144209858469968754; // pi/2
	start = -2.3561944901923449288469825374596271631478770495313; // -pi*3/4
	end = 2.3561944901923449288469825374596271631478770495313; // pi*3/4 
	start = -6.2831853071795864769252867665590057683943387987502;
	end = 6.2831853071795864769252867665590057683943387987502;
	start = -50;
	end = 50;
	start = -3.1415926535897932384626433832795028841971693993751; // -pi;
	end = 3.1415926535897932384626433832795028841971693993751; // pi;
	start = 0;
	end = 1;
	s.l = 0x0010000000000000;
	start = s.d;
	start = 3.1415926;
	end =   3.1415927;
	start = 10;
	end = 13;
	end = 4.9087385212340519350978802863742232565580771865236e-2; // pi/64
	end = 2.4543692606170259675489401431871116282790385932618e-2; // pi/128
	end = 3.1415926535897932384626433832795028841971693993751; // pi;
	end = 0.69314718055994530941723212145818;
	start = -100;
	end = 100;
	if(argc == 3) {
		start = atof(argv[1]);
		end = atof(argv[2]);
	}
	data_gen(start, end, DATA_COUNT);
}
