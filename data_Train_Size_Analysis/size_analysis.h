#ifndef __size_analysis_h__
#define __size_analysis_h__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TH1F.h>
#include <TFile.h>

// #define __debug__

int get_bcid(std::string);
std::string gtob(std::string);

class data_train
{
private:
	std::vector<std::string> spp_train;

public:
	data_train();
	~data_train();

	void add_spp(std::string);
	int get_size();
	void reset();

};

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cout << "Incorrect Arguments, see source for help" << std::endl;
		return -1;
	}

	/* Write root file */
	TFile 	* out_file = new TFile("data_train_length_analysis.root","RECREATE");
	TH1F	* overflow_histo_0 = new TH1F(
		"overflow_histo_0",
		"overflow histo 0",
		208,0,623);
	TH1F	* overflow_histo_50 = new TH1F(
		"overflow_histo_50",
		"overflow histo 50",
		208,0,623);
	TH1F	* overflow_histo_100 = new TH1F(
		"overflow_histo_100",
		"overflow histo 100",
		208,0,623);
	TH1F	* overflow_histo_150 = new TH1F(
		"overflow_histo_150",
		"overflow histo 150",
		208,0,623);
	TH1F	* overflow_histo_200 = new TH1F(
		"overflow_histo_200",
		"overflow histo 200",
		208,0,623);

	//Event counting
	int count_total(0);
	int count_50(0);
	int count_100(0);
	int count_150(0);
	int count_200(0);

	for (int i(0); i < 624; i++)
	{
		std::stringstream filename;
		filename << argv[1] << "timesync" << i << ".txt";

		#ifdef __debug__
			std::cout << "opening file: "<< filename.str() << std::endl;
		#endif

		std::cout << "file: " << i << std::endl;

		std::fstream * in_file = new std::fstream(filename.str().c_str());

		std::stringstream histoName;
		histoName << "timesync_"<< i << "_datatrain_analysis";
		TH1F * histo = new TH1F(
			histoName.str().c_str(),
			histoName.str().c_str(),
			250,0,250
			);

		/* Get data from file and input to histo */
		std::string buffer;
		int bcid_last(-1);
		data_train data;
		bool wait_on = true;
		while(*in_file >> buffer)
		{
			#ifdef __debug__
				std::cout << buffer << " : " << get_bcid(buffer) << std::endl;
			#endif

			int bcid = get_bcid(buffer);

			if (bcid != bcid_last && data.get_size() != 0)
			{
				#ifdef __debug__
					std::cout << "BCID Change" << std::endl;
				#endif

				// if (bcid_last - bcid > 400 && wait_on)
				// {
				// 	wait_on = false;
				// 	std::cout << "start: " << count << " bcid: " << bcid<< std::endl;
				// }
				// else  if (bcid_last - bcid > 400 && !wait_on)
				// {
				// 	std::cout << "end: " << count << " bcid: " << bcid<< std::endl;
				// 	break;
				// }

				// if (wait_on) 
				// {
				// 	data.reset();
				// 	bcid_last = bcid;
				// 	continue;
				// }

				histo->Fill(data.get_size());

				if (data.get_size() > 200)
				{
					overflow_histo_200->Fill(i);
					count_200++;
				}
				if (data.get_size() > 150)
				{
					overflow_histo_150->Fill(i);
					count_150++;
				}
				if (data.get_size() > 100)
				{
					overflow_histo_100->Fill(i);
					count_100++;
				}
				if (data.get_size() > 50)
				{
					overflow_histo_50->Fill(i);
					count_50++;
				}
				overflow_histo_0->Fill(i);

				data.reset();
				count_total ++;
			}

			data.add_spp(buffer);
			bcid_last = bcid;
		}

	
		out_file->cd();
		histo->Write();	
	}

	overflow_histo_200->Write();
	overflow_histo_150->Write();
	overflow_histo_100->Write();
	overflow_histo_50->Write();
	overflow_histo_0->Write();
	out_file->Close();
	

	std::cout << "Total Number of Data Trains: " << count_total << std::endl
		<< "Data Train Length > 50  : " << count_50 << " : " << double(count_50)*100/count_total << "%"<< std::endl
		<< "Data Train Length > 100 : " << count_100 << " : " << double(count_100)*100/count_total << "%"<< std::endl
		<< "Data Train Length > 150 : " << count_150 << " : " << double(count_150)*100/count_total << "%"<< std::endl
		<< "Data Train Length > 200 : " << count_200 << " : " << double(count_200)*100/count_total << "%"<< std::endl;
	// for (int i(0); i < 10; i++)
	// 	std::cout << "Blast Off!" << std::endl;
}

// ./prog dir_of_source filenumber

#endif 