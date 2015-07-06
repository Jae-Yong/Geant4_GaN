#ifndef __ANALYSIS_MANAVER_HH__
#define __ANALYSIS_MANAVER_HH__

#include <string>
//#include <unordered_map>
#include <map>

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

class AnalysisManager
{
private:
	static AnalysisManager* instance;

	TFile* mFile;

	std::map<std::string, TH1*>* th1Map;
	std::map<std::string, TH2*>* th2Map;

	AnalysisManager();
	AnalysisManager(const AnalysisManager& obj);
	~AnalysisManager();

public:
	static AnalysisManager* GetInstance()
	{
		if (instance == NULL)
			instance = new AnalysisManager();

		return instance;
	}

	static void Destroy()
	{
		if (instance)
		{
			delete instance;
			instance = NULL;
		}
	}

	void CreateHisto(std::string name, std::string title,
						int		nbinsx,
						double	xlow,
						double	xup);

	void CreateHisto(std::string name, std::string title,
						int		nbinsx,
						double	xlow,
						double	xup,
						int		nbinsy,
						double	ylow,
						double	yup);

	void Fill(std::string name, double val);
	void Fill(std::string name, double x, double val);
	void Fill(std::string name, double x, double y, double val);

	void Save();
};

#endif // __ANALYSIS_MANAVER_HH__
