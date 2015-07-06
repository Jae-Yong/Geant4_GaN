#include <cstring>
#include <fstream>

using namespace std;

void save(void)
{
	TFile* file = new TFile("GaN.root");
	TList* keys = file->GetListOfKeys();

	for (int i = 0; i < keys->GetSize(); i++)
	{
		TKey* key = (TKey*) keys->At(i);
		TObject* obj = key->ReadObj();
		char* objName = obj->ClassName();
		if (!strcmp(objName, "TH1F"))
		{
			TH1* histo = (TH1*) obj;
			int nbinsX = histo->GetNbinsX();

			ofstream dataFile((string(histo->GetName()) + string(".txt")).c_str());

			for (int j = 0; j < nbinsX; j++)
			{
				float binLowEdge = histo->GetXaxis()->GetBinLowEdge(j);
				int content = histo->GetBinContent(j);
				dataFile << binLowEdge << ":" << content << endl;
			}

			dataFile.close();
		}
		else if (!strcmp(objName, "TCanvas"))
		{
			TCanvas* can = (TCanvas*) obj;
			can->SaveAs("total.png");
		}
	}

	return; 
}