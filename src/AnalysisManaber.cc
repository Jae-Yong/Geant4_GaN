#include "AnalysisManager.hh"

#include "TCanvas.h"
#include "TPad.h"

#include "G4ios.hh"

using namespace std;

AnalysisManager* AnalysisManager::instance = NULL;

AnalysisManager::AnalysisManager()
	: mFile(NULL), th1Map(NULL), th2Map(NULL)
{

}

AnalysisManager::AnalysisManager(const AnalysisManager&)
{ }

AnalysisManager::~AnalysisManager()
{
	if (mFile)
	{
		mFile->Close();
		delete mFile;
		mFile = NULL;
	}
}

void AnalysisManager::CreateHisto(std::string name, std::string title,
					int		nbinsx,
					double	xlow,
					double	xup)
{
	if (th1Map == NULL)
		th1Map = new map<string, TH1*>();

	if (th1Map->find(name) == th1Map->end())
		(*th1Map)[name] = new TH1F(name.c_str(),
									title.c_str(),
									nbinsx,
									xlow,
									xup);
}

void AnalysisManager::CreateHisto(std::string name, std::string title,
					int		nbinsx,
					double	xlow,
					double	xup,
					int		nbinsy,
					double	ylow,
					double	yup)
{
	if (th2Map == NULL)
		th2Map = new map<string, TH2*>();

	if (th2Map->find(name) == th2Map->end())
		(*th2Map)[name] = new TH2F(name.c_str(),
									title.c_str(),
									nbinsx,
									xlow,
									xup,
									nbinsy,
									ylow,
									yup);
}

void AnalysisManager::Fill(std::string name, double val)
{
	if (th1Map == NULL || th1Map->find(name) == th1Map->end())
		return;

	(*th1Map)[name]->Fill(val);
}

void AnalysisManager::Fill(std::string name, double x, double val)
{
	if (th1Map == NULL || th1Map->find(name) == th1Map->end())
		return;

	(*th1Map)[name]->Fill(x, val);
}

void AnalysisManager::Fill(std::string name, double x, double y, double val)
{
	if (th2Map == NULL || th2Map->find(name) == th2Map->end())
		return;

	(*th2Map)[name]->Fill(x, y, val);
}


void AnalysisManager::Save()
{
	mFile = new TFile("GaN.root", "recreate");

	if (th1Map != NULL)
	{
		TCanvas* can = new TCanvas("Canvas", "Canvas", 800, 600);
		TPad** pads = new TPad*[th1Map->size()];
		int padNum = 0;

		for (map<string, TH1*>::iterator it = th1Map->begin(); it != th1Map->end(); it++, padNum++)
		{
			TH1* histo = it->second;

			double xlow = 1. / th1Map->size() * padNum;
			double xup  = 1. / th1Map->size() * (padNum + 1);

			pads[padNum] = new TPad(histo->GetName(), histo->GetTitle(),
										xlow,
										0,
										xup,
										1);
			pads[padNum]->Draw();
		}

		padNum = 0;
		for (map<string, TH1*>::iterator it = th1Map->begin(); it != th1Map->end(); it++, padNum++)
		{
			TH1* histo = it->second;

			pads[padNum]->cd();

			histo->Draw();
			histo->Write();
		}

		can->Write();
		delete can;
	}

	if (th2Map != NULL)
	{
		for (map<string, TH2*>::iterator it = th2Map->begin(); it != th2Map->end(); it++)
		{
			it->second->Write();
		}
	}

	mFile->Close();

	if (mFile != NULL)
	{
		delete mFile;
		mFile = NULL;
	}

	if (th1Map != NULL)
	{
		for (map<string, TH1*>::iterator it = th1Map->begin(); it != th1Map->end(); it++)
			delete it->second;
		delete th1Map;
		th1Map = NULL;
	}

	if (th2Map != NULL)
	{
		for (map<string, TH2*>::iterator it = th2Map->begin(); it != th2Map->end(); it++)
			delete it->second;
		delete th2Map;
		th2Map = NULL;
	}
}
