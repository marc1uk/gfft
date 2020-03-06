#include "gfft.h"
/* article on dr dobbs: https://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857
source code on p2 of article part II (link at end of part 1)
*/
#include <complex>
#include <vector>

const int P=2;
typedef double Tp;

AbstractFFT<double>* GetGfft(int p);
void GfftCPU(std::vector<std::complex<double> > &values);

int main(){
	
	std::vector<std::complex<double> > thedata;
	
	// sample data
	unsigned long n = 1<<P;
	Tp* data = new Tp [2*n];
	for (uint i=0; i<n; ++i) {
		thedata.push_back(std::complex<double>(2*i,2*i+1));
	}
	
	cout<<"input\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	for(uint i=0; i<thedata.size(); ++i){
		std::complex<double> aval = thedata.at(i);
		cout << "\t" << aval.real() << "\t" << aval.imag() << "i\n";
	}
	
	// compute FFT
	GfftCPU(thedata);
	
	cout<<"\noutput\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	for(uint i=0; i<thedata.size(); ++i){
		std::complex<double> aval = thedata.at(i);
		cout << "\t" << aval.real() << "\t" << aval.imag() << "i\n";
	}
	
	delete [] data;
	
	return 0;
}

void GfftCPU(std::vector<std::complex<double> > &values){
	
	static AbstractFFT<Tp>* gfft = NULL;
	if(gfft==NULL){
		gfft = GetGfft(P);
	}
	
	// main reason for this wrapper is that the data has to be in a weird format (for SPEEED):
	// even datapoints represent real parts, odd ones represent imaginary parts
	unsigned long n = 1<<P;  // bitshift way of doing 2^p... we could just use values.size()
	
	// copy the data into the new format array (trash those speed optimizations!)
	static Tp* data = new double[2*n];
	for (uint i=0; i<n; ++i){
		data[2*i] = values.at(i).real();
		data[2*i+1] = values.at(i).imag();
	}
	
	// compute FFT
	gfft->fft(data);
	
	// copy back the results
	for (uint i=0; i<n; ++i){
		values.at(i) = std::complex<Tp>(data[2*i], data[2*i+1]);
	}
	
	//delete [] data;  // either make/delete it every time, or use static and leak the last one.
	// or, add some way to indicate last iteration and cleanup
}

AbstractFFT<double>* GetGfft(int p){
	
	// range of the needed GFFT classes ...
	// https://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199702312?pgno=2
	// ... i think... it needs to compile each size of FFT separately
	// to allow runtime variation, we pass it a range of potential sizes
	// and it builds the required FFTs.
	// with decimationfactor=1 (max size), we need 14 bits... not sure if this means
	// we need all FFT sizes from 1-14, or just 14...
	// ... it also seems like maybe you can use gfft directly without the factory
	// by using FactoryPolicy=EmptyFFT... somehow...
	// 
	// ...
	// for now leave it as per example
	const unsigned Min = 1;
	const unsigned Max = 27;
	
	// initialization of the object factory
	Loki::Factory<AbstractFFT<Tp>,unsigned int> gfft_factory;
	FactoryInit<GFFTList<GFFT,Min,Max>::Result>::apply(gfft_factory);
	
	// create an instance of the GFFT
	AbstractFFT<Tp>* gfft = gfft_factory.CreateObject(p);
	
	return gfft;
}
