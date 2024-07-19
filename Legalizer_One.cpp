#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>

using namespace std;

struct XYNameCell {
    float XCoord;
    float YCoord;
    string Name;
};

struct InstRes{
    string NInst;
    string NRes;
};

float TwoD_to_OneD(float CurX, float CurY){
    //CurX and CurY must become int, not float.
    //Using float to make it easier for datatypes
    float MaxX = 2000;
    float DigestCurX = CurX*10;
    float DigestCurY = CurY*10;
    float CurrentValue = DigestCurY*MaxX+DigestCurX;
    //cout<<CurrentValue<<" ";
    return CurrentValue;
}

float OneD_to_TwoDX(float TargetValue){
    int MaxX = 2000;
    int CurValue = TargetValue;
    int CurX = CurValue%MaxX;
    int CurY = CurValue/MaxX;
    float fCurX = CurX;
    float fCurY = CurY;
    float RXAnswer = fCurX/10;
    float RYAnswer = fCurY/10;
    return RXAnswer;
}

float OneD_to_TwoDY(float TargetValue){
    int MaxX = 2000;
    int CurValue = TargetValue;
    int CurX = CurValue%MaxX;
    int CurY = CurValue/MaxX;
    float fCurX = CurX;
    float fCurY = CurY;
    float RXAnswer = fCurX/10;
    float RYAnswer = fCurY/10;
    return RYAnswer;
}

vector<float> GetWeight(vector<float>&Legals, vector<float>&CurFilleds, vector<float>&Maxs){
    vector<float> CurWeight;
    for(int i=0; i<Legals.size(); i++) // Itterate all Legals
    {
        float CurrentAmount = CurFilleds[i];
        float MaxAmount = Maxs[i];
        float Weight = 0;
        float WeightedWeight = (MaxAmount-CurrentAmount)/MaxAmount; //For Backup
        if(CurrentAmount<MaxAmount)
            Weight = 1;
        else
            Weight = 10000;
        CurWeight.push_back(Weight);
    }
    return CurWeight;
}

float FindNearestYCoord(float CurY, vector<float>&LegalYs, vector<float>&CurFilledYs, vector<float>&MaxYs){
    float AnswerY;
    vector<float> CurWeight = GetWeight(LegalYs, CurFilledYs, MaxYs);
    AnswerY = 0;
    int CurrentMaxSelect = 0;
    float CurrentMin = 10000;

    float CurEquation;
    for(int i=0; i<LegalYs.size(); i++) // Find Closest Y
    {
        CurEquation = 0;
        CurEquation = (LegalYs[i]-CurY)*CurWeight[i];
        CurEquation = abs(CurEquation);

        if(CurEquation<=CurrentMin)
        {
            CurrentMin = CurEquation;
            CurrentMaxSelect = i;
        }

    }
    AnswerY = LegalYs[CurrentMaxSelect];
    CurFilledYs[CurrentMaxSelect]+=1;
    return AnswerY;
}

bool CompStructX(const XYNameCell &a, const XYNameCell &b){
    return a.XCoord<b.XCoord;
}

bool CompStructY(const XYNameCell &a, const XYNameCell &b){
    return a.YCoord<b.YCoord;
}

vector<XYNameCell>SortBasedX(vector<XYNameCell>LegalYVector){
    sort(LegalYVector.begin(), LegalYVector.end(), CompStructX);
    return LegalYVector;
}

vector<XYNameCell>SortBasedY(vector<XYNameCell>NonLegalYVector){
    sort(NonLegalYVector.begin(), NonLegalYVector.end(), CompStructY);
    return NonLegalYVector;
}

vector<XYNameCell>LegalizeYVector(vector<XYNameCell>&RawVec, vector<float>&LegalYs,
                                  vector<float>&CurFilledYs, vector<float>&MaxYs) {
    vector<XYNameCell> VecCellLegalY;
    vector<XYNameCell>SortVector = SortBasedY(RawVec);

    for(int i=0; i<SortVector.size();i++)
    {
        XYNameCell NewCell;
        NewCell.XCoord = SortVector[i].XCoord;
        NewCell.Name = SortVector[i].Name;
        float CurY = SortVector[i].YCoord;
        CurY = FindNearestYCoord(CurY, LegalYs, CurFilledYs, MaxYs);
        NewCell.YCoord = CurY;
        VecCellLegalY.push_back(NewCell);
    }
    return VecCellLegalY;
}

vector<XYNameCell>LegalizeXVector(vector<XYNameCell>&LegalYVector, vector<float>&LegalXs,
                                  map<float,float>&CurFilledXByYs){
    vector<XYNameCell>TempAns;

    vector<XYNameCell>SortVector = SortBasedX(LegalYVector);

    for(int i=0; i<SortVector.size(); i++)
    {
        XYNameCell NewCell;
        NewCell.Name = SortVector[i].Name;
        NewCell.YCoord = SortVector[i].YCoord;

        float CurrentCount = CurFilledXByYs[SortVector[i].YCoord];
        NewCell.XCoord = LegalXs[CurrentCount];
        //cout<<CurrentCount<<" ";
        CurFilledXByYs[SortVector[i].YCoord]+=1;
        //cout<<LegalXs.size()<<"]";
        TempAns.push_back(NewCell);
    }
    return TempAns;
}

vector<InstRes>PairInstanceResource(vector<XYNameCell>&LegalizedXY, map<float,string>&MapArcODName) {
    vector<InstRes>TempAns;
    for(int i=0; i<LegalizedXY.size(); i++)
    {
        InstRes TempInstRes;
        float CurX = LegalizedXY[i].XCoord;
        float CurY = LegalizedXY[i].YCoord;
        string CurName = LegalizedXY[i].Name;
        float OneDRes = TwoD_to_OneD(CurX, CurY);
        string ResName = MapArcODName[OneDRes];
        //cout<<"Map0D: " << MapArcODName[OneDRes]<<endl;
        TempInstRes.NInst = CurName;
        TempInstRes.NRes = ResName;
        TempAns.push_back(TempInstRes);
    }
    return TempAns;
}

int main(int argc, char* argv[])
{
    //Preping Variable (Below) || Until: Finish Preping Variable
    cout << "Hello World!" << endl;
    //Initialize Vectors
    vector<string>ArcResourceName;
    vector<string>ArcResourceType;
    vector<float>ArcResourceX;
    vector<float>ArcResourceY;
    vector<string>InsResourceName;
    vector<string>InsResourceType;
    vector<float>InsResourceX;
    vector<float>InsResourceY;


    cout<<"Inputs: "<<endl;
    //Preparing for Input
    string InputTextArchitecture;
    //InputTextArchitecture ="testcase1/architecture.txt";
    InputTextArchitecture = argv[1];
    cout<<argv[1]<<endl;
    ifstream ArchitectureText(InputTextArchitecture);
    //ifstream ArchitectureText("testcase1/arctest2.txt"); //Testing Arc

    string InputTextInstance;
    //InputTextInstance = "testcase1/instance.txt";
    InputTextInstance = argv[2];
    cout<<argv[2]<<endl;
    ifstream InstanceText(InputTextInstance);

    string InputTextNetlist;
    //InputTextNetlist = "testcase1/netlist.txt";
    InputTextNetlist = argv[3];
    cout<<argv[3]<<endl;
    ifstream NetlistText(InputTextNetlist);

    string InputOutputName;
    //InputOutputName = "output.txt";
    InputOutputName = argv[4];
    cout<<argv[4]<<endl;

    //Check for No Input
    if(!ArchitectureText) cout << "No Input for Architecture!" <<endl;
    if(!InstanceText) cout << "No Input For Instance!" <<endl;
    if(!NetlistText) cout << "No Input for Netlist!" <<endl;

    //========================================Getting the Architecture -> Vector
    int LineCounterArchitecture = 0;
    int TotalInputLinesArchitecture = 0;
    while(getline(ArchitectureText, InputTextArchitecture))
    {
        //cout<< InputTextArchitecture << endl;
        stringstream SS(InputTextArchitecture);
        string ChosenWord;

        int OrderingCount=0;
        while(SS >> ChosenWord)
        {
            //ResetCount
            if(OrderingCount>=4)
            {
                OrderingCount=0;
            }
            //cout<<ChosenWord<<endl;
            if(OrderingCount==0) //Resource-Name
            {
                //cout<<"ResourceName: " << ChosenWord <<endl;
                ArcResourceName.push_back(ChosenWord);
            }
            else if(OrderingCount==1) //Resource-Type
            {
                //cout<<"ResourceType: " << ChosenWord <<endl;
                ArcResourceType.push_back(ChosenWord);
            }
            else if(OrderingCount==2) //X-Coords
            {
                //cout<<"ResourceXCoords: " << ChosenWord <<endl;
                ArcResourceX.push_back(stof(ChosenWord));
            }
            else if(OrderingCount==3) //Y-Coords
            {
                //cout<<"ResourceYCoords: " << ChosenWord <<endl;
                ArcResourceY.push_back(stof(ChosenWord));
                //cout<<"========"<<endl;
            }
            OrderingCount+=1;
        }

        LineCounterArchitecture += 1;
        TotalInputLinesArchitecture+=1;
        //if(LineCounterArchitecture>=5)
        //    break;
    }


    //=======================================Getting the Instance -> Vector
    int LineCounterInstance = 0;
    int TotalInputLinesInstance = 0;
    while(getline(InstanceText, InputTextInstance))
    {
        stringstream SS(InputTextInstance);
        string ChosenWord;

        int OrderingCount=0;
        while(SS >> ChosenWord)
        {
            //ResetCount
            if(OrderingCount>=4)
            {
                OrderingCount=0;
            }
            //cout<<ChosenWord<<endl;
            if(OrderingCount==0) //Resource-Name
            {
                //cout<<"ResourceName: " << ChosenWord <<endl;
                InsResourceName.push_back(ChosenWord);
            }
            else if(OrderingCount==1) //Resource-Type
            {
                //cout<<"ResourceType: " << ChosenWord <<endl;
                InsResourceType.push_back(ChosenWord);
            }
            else if(OrderingCount==2) //X-Coords
            {
                //cout<<"ResourceXCoords: " << ChosenWord <<endl;
                InsResourceX.push_back(stof(ChosenWord));
            }
            else if(OrderingCount==3) //Y-Coords
            {
                //cout<<"ResourceYCoords: " << ChosenWord <<endl;
                InsResourceY.push_back(stof(ChosenWord));
                //cout<<"========"<<endl;
            }
            OrderingCount+=1;
        }

        LineCounterInstance += 1;
        TotalInputLinesInstance+=1;
        //if(LineCounterInstance>=5)
        //    break;
    }

    cout<<endl;
    cout<<"==========="<<endl;
    cout<< "Finish Reading"<<endl;
    cout<< "========="<<endl;
    ArchitectureText.close();
    InstanceText.close();
    NetlistText.close();



    cout<<"Architecture: "<< endl;

    /*
    for(int i=0; i<TotalInputLinesArchitecture; i++)
    {
        cout<<ArcResourceName.at(i) << endl;
        cout<<ArcResourceType.at(i) << endl;
        cout<<ArcResourceX.at(i) << endl;
        cout<<ArcResourceY.at(i) << endl;
        cout<<endl;
    }*/


    //The Size Should all be the same
    cout<<ArcResourceName.size() << endl;
    cout<<ArcResourceType.size() << endl;
    cout<<ArcResourceX.size() << endl;
    cout<<ArcResourceY.size() << endl;
    cout<<TotalInputLinesArchitecture<<endl;

    /*
    cout<<"==========="<<endl;
    cout<< "Seperator"<<endl;
    cout<< "========="<<endl;
    */
    cout<< endl;

    cout<<"Instances: "<< endl;
    /*
    for(int i=0; i<TotalInputLinesInstance; i++)
    {
        cout<<InsResourceName.at(i) << endl;
        cout<<InsResourceType.at(i) << endl;
        cout<<InsResourceX.at(i) << endl;
        cout<<InsResourceY.at(i) << endl;
        cout<<endl;
    }*/

    //The Size Should all be the same
    cout<<InsResourceName.size() << endl;
    cout<<InsResourceType.size() << endl;
    cout<<InsResourceX.size() << endl;
    cout<<InsResourceY.size() << endl;
    cout<<TotalInputLinesInstance<<endl;

    cout<<"==========="<<endl;
    cout<< "Finish Printing for Arc & Ins"<<endl;
    cout<< "========="<<endl;


    //=======================================Getting X, Y-Coords (Based on CLB,RAM,DSP)
    //========Architecture Turn
    /*
    //Initialize Vectors
    vector<string>ArcResourceName;
    vector<string>ArcResourceType;
    vector<float>ArcResourceX;
    vector<float>ArcResourceY;
    */

    vector<string>ArcResourceCLBName;
    vector<float>ArcResourceCLBX;
    vector<float>ArcResourceCLBY;
    vector<string>ArcResourceRAMName;
    vector<float>ArcResourceRAMX;
    vector<float>ArcResourceRAMY;
    vector<string>ArcResourceDSPName;
    vector<float>ArcResourceDSPX;
    vector<float>ArcResourceDSPY;
    int TotalArcResourceCLB = 0;
    int TotalArcResourceRAM = 0;
    int TotalArcResourceDSP = 0;

    vector<XYNameCell> VecXYNameArcCLB;
    vector<XYNameCell> VecXYNameArcRAM;
    vector<XYNameCell> VecXYNameArcDSP;

    map<string,float>MapArcNameX;
    map<string,float>MapArcNameY;
    map<float,string>MapArcODName;

    for(int i=0; i<TotalInputLinesArchitecture; i++)
    {
        if(ArcResourceType.at(i)=="CLB")
        {
            //Go to ArcResourceCLB
            XYNameCell CurCell;
            CurCell.XCoord = ArcResourceX.at(i);
            CurCell.YCoord = ArcResourceY.at(i);
            CurCell.Name = ArcResourceName.at(i);
            VecXYNameArcCLB.push_back(CurCell);
            MapArcNameX.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceX.at(i)));
            MapArcNameY.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceY.at(i)));
            float OneDValue = TwoD_to_OneD(ArcResourceX.at(i), ArcResourceY.at(i));
            //cout<<OneDValue<< " ";
            MapArcODName.insert(pair<float,string>(OneDValue, ArcResourceName.at(i)));
            ArcResourceCLBName.push_back(ArcResourceName.at(i));
            ArcResourceCLBX.push_back(ArcResourceX.at(i));
            ArcResourceCLBY.push_back(ArcResourceY.at(i));
            TotalArcResourceCLB += 1;
        }
        else if(ArcResourceType.at(i)=="RAM")
        {
            //Go to ArcResourceRAM
            XYNameCell CurCell;
            CurCell.XCoord = ArcResourceX.at(i);
            CurCell.YCoord = ArcResourceY.at(i);
            CurCell.Name = ArcResourceName.at(i);
            VecXYNameArcRAM.push_back(CurCell);
            MapArcNameX.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceX.at(i)));
            MapArcNameY.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceY.at(i)));
            float OneDValue = TwoD_to_OneD(ArcResourceX.at(i), ArcResourceY.at(i));
            //cout<<OneDValue<< " ";
            MapArcODName.insert(pair<float,string>(OneDValue, ArcResourceName.at(i)));
            ArcResourceRAMName.push_back(ArcResourceName.at(i));
            ArcResourceRAMX.push_back(ArcResourceX.at(i));
            ArcResourceRAMY.push_back(ArcResourceY.at(i));
            TotalArcResourceRAM += 1;
        }
        else if(ArcResourceType.at(i)=="DSP")
        {
            //Go to ArcResourceDSP
            XYNameCell CurCell;
            CurCell.XCoord = ArcResourceX.at(i);
            CurCell.YCoord = ArcResourceY.at(i);
            CurCell.Name = ArcResourceName.at(i);
            VecXYNameArcDSP.push_back(CurCell);
            MapArcNameX.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceX.at(i)));
            MapArcNameY.insert(pair<string,float>(ArcResourceName.at(i), ArcResourceY.at(i)));
            float OneDValue = TwoD_to_OneD(ArcResourceX.at(i), ArcResourceY.at(i));
            //cout<<OneDValue<< " ";
            MapArcODName.insert(pair<float,string>(OneDValue, ArcResourceName.at(i)));
            ArcResourceDSPName.push_back(ArcResourceName.at(i));
            ArcResourceDSPX.push_back(ArcResourceX.at(i));
            ArcResourceDSPY.push_back(ArcResourceY.at(i));
            TotalArcResourceDSP += 1;
        }
    }

    cout<< "Counting Total Arc Resource:" <<endl;
    cout<< "CLB: " << TotalArcResourceCLB << endl;
    cout<< "RAM: " << TotalArcResourceRAM << endl;
    cout<< "DSP: " << TotalArcResourceDSP << endl;
    cout<< "VecCLB: " << VecXYNameArcCLB.size()<<endl;
    cout<< "VecRAM: " << VecXYNameArcRAM.size()<<endl;
    cout<< "VecDSP: " << VecXYNameArcDSP.size()<<endl;
    cout<< "MapX: " << MapArcNameX.size()<<endl;
    cout<< "MapY: " << MapArcNameY.size()<<endl;
    cout<< "Map1D: " << MapArcODName.size()<<endl;


    cout<<endl;
    //========Instance Turn
    /*
    vector<string>InsResourceName;
    vector<string>InsResourceType;
    vector<float>InsResourceX;
    vector<float>InsResourceY;
    */

    vector<string>InsResourceCLBName;
    vector<float>InsResourceCLBX;
    vector<float>InsResourceCLBY;
    vector<string>InsResourceRAMName;
    vector<float>InsResourceRAMX;
    vector<float>InsResourceRAMY;
    vector<string>InsResourceDSPName;
    vector<float>InsResourceDSPX;
    vector<float>InsResourceDSPY;
    vector<string>InsResourceIOName;
    vector<float>InsResourceIOX;
    vector<float>InsResourceIOY;
    int TotalInsResourceCLB = 0;
    int TotalInsResourceRAM = 0;
    int TotalInsResourceDSP = 0;
    int TotalInsResourceIO = 0;

    vector<XYNameCell> VecXYNameInstCLB;
    vector<XYNameCell> VecXYNameInstRAM;
    vector<XYNameCell> VecXYNameInstDSP;
    vector<XYNameCell> VecXYNameInstIO;

    map<string,float>MapInstNameX;
    map<string,float>MapInstNameY;

    for(int i=0; i<TotalInputLinesInstance; i++)
    {
        if(InsResourceType.at(i)=="CLB")
        {
            //Go to ArcResourceCLB
            XYNameCell CurCell;
            CurCell.XCoord = InsResourceX.at(i);
            CurCell.YCoord = InsResourceY.at(i);
            CurCell.Name = InsResourceName.at(i);
            VecXYNameInstCLB.push_back(CurCell);
            MapInstNameX.insert(pair<string,float>(InsResourceName.at(i), InsResourceX.at(i)));
            MapInstNameY.insert(pair<string,float>(InsResourceName.at(i), InsResourceY.at(i)));
            InsResourceCLBName.push_back(InsResourceName.at(i));
            InsResourceCLBX.push_back(InsResourceX.at(i));
            InsResourceCLBY.push_back(InsResourceY.at(i));
            TotalInsResourceCLB += 1;
        }
        else if(InsResourceType.at(i)=="RAM")
        {
            //Go to ArcResourceRAM
            XYNameCell CurCell;
            CurCell.XCoord = InsResourceX.at(i);
            CurCell.YCoord = InsResourceY.at(i);
            CurCell.Name = InsResourceName.at(i);
            VecXYNameInstRAM.push_back(CurCell);
            MapInstNameX.insert(pair<string,float>(InsResourceName.at(i), InsResourceX.at(i)));
            MapInstNameY.insert(pair<string,float>(InsResourceName.at(i), InsResourceY.at(i)));
            InsResourceRAMName.push_back(InsResourceName.at(i));
            InsResourceRAMX.push_back(InsResourceX.at(i));
            InsResourceRAMY.push_back(InsResourceY.at(i));
            TotalInsResourceRAM += 1;
        }
        else if(InsResourceType.at(i)=="DSP")
        {
            //Go to ArcResourceDSP
            XYNameCell CurCell;
            CurCell.XCoord = InsResourceX.at(i);
            CurCell.YCoord = InsResourceY.at(i);
            CurCell.Name = InsResourceName.at(i);
            VecXYNameInstDSP.push_back(CurCell);
            MapInstNameX.insert(pair<string,float>(InsResourceName.at(i), InsResourceX.at(i)));
            MapInstNameY.insert(pair<string,float>(InsResourceName.at(i), InsResourceY.at(i)));
            InsResourceDSPName.push_back(InsResourceName.at(i));
            InsResourceDSPX.push_back(InsResourceX.at(i));
            InsResourceDSPY.push_back(InsResourceY.at(i));
            TotalInsResourceDSP += 1;
        }
        else if(InsResourceType.at(i)=="IO")
        {
            //Go to ArcResourceIO
            XYNameCell CurCell;
            CurCell.XCoord = InsResourceX.at(i);
            CurCell.YCoord = InsResourceY.at(i);
            CurCell.Name = InsResourceName.at(i);
            VecXYNameInstIO.push_back(CurCell);
            MapInstNameX.insert(pair<string,float>(InsResourceName.at(i), InsResourceX.at(i)));
            MapInstNameY.insert(pair<string,float>(InsResourceName.at(i), InsResourceY.at(i)));
            InsResourceIOName.push_back(InsResourceName.at(i));
            InsResourceIOX.push_back(InsResourceX.at(i));
            InsResourceIOY.push_back(InsResourceY.at(i));
            TotalInsResourceIO += 1;
        }
    }

    cout<< "Counting Total Arc Resource:" <<endl;
    cout<< "CLB: " << TotalInsResourceCLB << endl;
    cout<< "RAM: " << TotalInsResourceRAM << endl;
    cout<< "DSP: " << TotalInsResourceDSP << endl;
    cout<< "IO: " << TotalInsResourceIO << endl;
    cout<< "VecCLB: " << VecXYNameInstCLB.size()<<endl;
    cout<< "VecRAM: " << VecXYNameInstRAM.size()<<endl;
    cout<< "VecDSP: " << VecXYNameInstDSP.size()<<endl;
    cout<< "VecIO: " << VecXYNameInstIO.size()<<endl;
    cout<< "MapX: " << MapInstNameX.size()<<endl;
    cout<< "MapY: " << MapInstNameY.size()<<endl;


    cout<<"==========="<<endl;
    cout<< "Finish Sorting to CLB,RAM,DSP,IO"<<endl;
    cout<< "========="<<endl;


    //=======================================Getting Unique X, Y-Coords of Arc (Based on CLB,RAM,DSP)

    set<float> SetArcResourceCLBX_Unique;
    set<float> SetArcResourceCLBY_Unique;
    set<float> SetArcResourceRAMX_Unique;
    set<float> SetArcResourceRAMY_Unique;
    set<float> SetArcResourceDSPX_Unique;
    set<float> SetArcResourceDSPY_Unique;

    for(int i=0; i<TotalArcResourceCLB; i++)
    {
        SetArcResourceCLBX_Unique.insert(ArcResourceCLBX.at(i));
        SetArcResourceCLBY_Unique.insert(ArcResourceCLBY.at(i));
    }
    for(int i=0; i<TotalArcResourceRAM; i++)
    {
        SetArcResourceRAMX_Unique.insert(ArcResourceRAMX.at(i));
        SetArcResourceRAMY_Unique.insert(ArcResourceRAMY.at(i));
    }
    for(int i=0; i<TotalArcResourceDSP; i++)
    {
        SetArcResourceDSPX_Unique.insert(ArcResourceDSPX.at(i));
        SetArcResourceDSPY_Unique.insert(ArcResourceDSPY.at(i));
    }

    /*
    for(float val : ArcResourceCLBX_Unique) {
        cout << val << " ";
    }*/

    vector<float>ArcResourceCLBX_Unique;
    vector<float>ArcResourceCLBY_Unique;
    vector<float>ArcResourceRAMX_Unique;
    vector<float>ArcResourceRAMY_Unique;
    vector<float>ArcResourceDSPX_Unique;
    vector<float>ArcResourceDSPY_Unique;

    for(float Coords : SetArcResourceCLBX_Unique) {
        ArcResourceCLBX_Unique.push_back(Coords);
    }
    for(float Coords : SetArcResourceCLBY_Unique) {
        ArcResourceCLBY_Unique.push_back(Coords);
    }
    for(float Coords : SetArcResourceRAMX_Unique) {
        ArcResourceRAMX_Unique.push_back(Coords);
    }
    for(float Coords : SetArcResourceRAMY_Unique) {
        ArcResourceRAMY_Unique.push_back(Coords);
    }
    for(float Coords : SetArcResourceDSPX_Unique) {
        ArcResourceDSPX_Unique.push_back(Coords);
    }
    for(float Coords : SetArcResourceDSPY_Unique) {
        ArcResourceDSPY_Unique.push_back(Coords);
    }

    /*
    cout<<endl;
    cout<<SetArcResourceCLBX_Unique.size()<<endl;
    cout<<SetArcResourceCLBY_Unique.size()<<endl;
    cout<<SetArcResourceRAMX_Unique.size()<<endl;
    cout<<SetArcResourceRAMY_Unique.size()<<endl;
    cout<<SetArcResourceDSPX_Unique.size()<<endl;
    cout<<SetArcResourceDSPY_Unique.size()<<endl;
    */

    cout<<"Total Uniques: "<<endl;
    cout<<"CLBX: " <<ArcResourceCLBX_Unique.size()<<endl;
    cout<<"CLBY: " <<ArcResourceCLBY_Unique.size()<<endl;
    cout<<"RAMX: " <<ArcResourceRAMX_Unique.size()<<endl;
    cout<<"RAMY: " <<ArcResourceRAMY_Unique.size()<<endl;
    cout<<"DSPX: " <<ArcResourceDSPX_Unique.size()<<endl;
    cout<<"DSPY: " <<ArcResourceDSPY_Unique.size()<<endl;

    cout<<"==========="<<endl;
    cout<< "Finish Getting Unique X,Y Coords from Architecture"<<endl;
    cout<< "========="<<endl;

    //=======================================Getting Unique X, Y-Coords of Arc (Based on CLB,RAM,DSP)

    vector<float>CountArcResourceCLBX_Unique;
    vector<float>CountArcResourceCLBY_Unique;
    vector<float>CountArcResourceRAMX_Unique;
    vector<float>CountArcResourceRAMY_Unique;
    vector<float>CountArcResourceDSPX_Unique;
    vector<float>CountArcResourceDSPY_Unique;

    vector<float>CountArcResourceCLBX_Unique_Counter;
    vector<float>CountArcResourceCLBY_Unique_Counter;
    vector<float>CountArcResourceRAMX_Unique_Counter;
    vector<float>CountArcResourceRAMY_Unique_Counter;
    vector<float>CountArcResourceDSPX_Unique_Counter;
    vector<float>CountArcResourceDSPY_Unique_Counter;

    for(int i=0; i<ArcResourceCLBX_Unique.size(); i++)
    {
        CountArcResourceCLBX_Unique.push_back(0);
        CountArcResourceCLBX_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceCLBX.size(); k++)
        {
            if(ArcResourceCLBX_Unique[i]==ArcResourceCLBX[k])
                CountArcResourceCLBX_Unique[i]+=1;
        }
    }
    for(int i=0; i<ArcResourceCLBY_Unique.size(); i++)
    {
        CountArcResourceCLBY_Unique.push_back(0);
        CountArcResourceCLBY_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceCLBY.size(); k++)
        {
            if(ArcResourceCLBY_Unique[i]==ArcResourceCLBY[k])
                CountArcResourceCLBY_Unique[i]+=1;
        }
    }

    for(int i=0; i<ArcResourceRAMX_Unique.size(); i++)
    {
        CountArcResourceRAMX_Unique.push_back(0);
        CountArcResourceRAMX_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceRAMX.size(); k++)
        {
            if(ArcResourceRAMX_Unique[i]==ArcResourceRAMX[k])
                CountArcResourceRAMX_Unique[i]+=1;
        }
    }
    for(int i=0; i<ArcResourceRAMY_Unique.size(); i++)
    {
        CountArcResourceRAMY_Unique.push_back(0);
        CountArcResourceRAMY_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceRAMY.size(); k++)
        {
            if(ArcResourceRAMY_Unique[i]==ArcResourceRAMY[k])
                CountArcResourceRAMY_Unique[i]+=1;
        }
    }

    for(int i=0; i<ArcResourceDSPX_Unique.size(); i++)
    {
        CountArcResourceDSPX_Unique.push_back(0);
        CountArcResourceDSPX_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceDSPX.size(); k++)
        {
            if(ArcResourceDSPX_Unique[i]==ArcResourceDSPX[k])
                CountArcResourceDSPX_Unique[i]+=1;
        }
    }
    for(int i=0; i<ArcResourceDSPY_Unique.size(); i++)
    {
        CountArcResourceDSPY_Unique.push_back(0);
        CountArcResourceDSPY_Unique_Counter.push_back(0);
        for(int k=0; k<ArcResourceDSPY.size(); k++)
        {
            if(ArcResourceDSPY_Unique[i]==ArcResourceDSPY[k])
                CountArcResourceDSPY_Unique[i]+=1;
        }
    }

    cout<<"Count Arc Uniques: (vectors)" <<endl;
    cout<<"CLBX: " << CountArcResourceCLBX_Unique.size()<<endl;
    cout<<"CLBY: " << CountArcResourceCLBY_Unique.size()<<endl;
    cout<<"RAMX: " << CountArcResourceRAMX_Unique.size()<<endl;
    cout<<"RAMY: " << CountArcResourceRAMY_Unique.size()<<endl;
    cout<<"DSPX: " << CountArcResourceDSPX_Unique.size()<<endl;
    cout<<"DSPY: " << CountArcResourceDSPY_Unique.size()<<endl;
    cout<<"CLBX_C: " << CountArcResourceCLBX_Unique_Counter.size()<<endl;
    cout<<"CLBY_C: " << CountArcResourceCLBY_Unique_Counter.size()<<endl;
    cout<<"RAMX_C: " << CountArcResourceRAMX_Unique_Counter.size()<<endl;
    cout<<"RAMY_C: " << CountArcResourceRAMY_Unique_Counter.size()<<endl;
    cout<<"DSPX_C: " << CountArcResourceDSPX_Unique_Counter.size()<<endl;
    cout<<"DSPY_C: " << CountArcResourceDSPY_Unique_Counter.size()<<endl;
    cout<<endl;

    map<float,float>MapArcCLBCountMaxX;
    map<float,float>MapArcCLBCountMaxY;
    map<float,float>MapArcRAMCountMaxX;
    map<float,float>MapArcRAMCountMaxY;
    map<float,float>MapArcDSPCountMaxX;
    map<float,float>MapArcDSPCountMaxY;

    map<float,float>MapArcCLBCountMaxX_Counter;
    map<float,float>MapArcCLBCountMaxY_Counter;
    map<float,float>MapArcRAMCountMaxX_Counter;
    map<float,float>MapArcRAMCountMaxY_Counter;
    map<float,float>MapArcDSPCountMaxX_Counter;
    map<float,float>MapArcDSPCountMaxY_Counter;

    for(int i=0; i<ArcResourceCLBX_Unique.size(); i++)
    {
        MapArcCLBCountMaxX.insert(pair<float,float>(ArcResourceCLBX_Unique[i], CountArcResourceCLBX_Unique[i]));
        MapArcCLBCountMaxX_Counter.insert(pair<float,float>(ArcResourceCLBX_Unique[i], 0));
    }
    for(int i=0; i<ArcResourceCLBY_Unique.size(); i++)
    {
        MapArcCLBCountMaxY.insert(pair<float,float>(ArcResourceCLBY_Unique[i], CountArcResourceCLBY_Unique[i]));
        MapArcCLBCountMaxY_Counter.insert(pair<float,float>(ArcResourceCLBY_Unique[i], 0));
    }
    for(int i=0; i<ArcResourceRAMX_Unique.size(); i++)
    {
        MapArcRAMCountMaxX.insert(pair<float,float>(ArcResourceRAMX_Unique[i], CountArcResourceRAMX_Unique[i]));
        MapArcRAMCountMaxX_Counter.insert(pair<float,float>(ArcResourceRAMX_Unique[i], 0));
    }
    for(int i=0; i<ArcResourceRAMY_Unique.size(); i++)
    {
        MapArcRAMCountMaxY.insert(pair<float,float>(ArcResourceRAMY_Unique[i], CountArcResourceRAMY_Unique[i]));
        MapArcRAMCountMaxY_Counter.insert(pair<float,float>(ArcResourceRAMY_Unique[i], 0));
    }
    for(int i=0; i<ArcResourceDSPX_Unique.size(); i++)
    {
        MapArcDSPCountMaxX.insert(pair<float,float>(ArcResourceDSPX_Unique[i], CountArcResourceDSPX_Unique[i]));
        MapArcDSPCountMaxX_Counter.insert(pair<float,float>(ArcResourceDSPX_Unique[i], 0));
    }
    for(int i=0; i<ArcResourceDSPY_Unique.size(); i++)
    {
        MapArcDSPCountMaxY.insert(pair<float,float>(ArcResourceDSPY_Unique[i], CountArcResourceDSPY_Unique[i]));
        MapArcDSPCountMaxY_Counter.insert(pair<float,float>(ArcResourceDSPY_Unique[i], 0));
    }

    cout<<"Map Now:"<<endl;
    cout<<"CLBX: " << MapArcCLBCountMaxX.size()<<endl;
    cout<<"CLBY: " << MapArcCLBCountMaxY.size()<<endl;
    cout<<"RAMX: " << MapArcRAMCountMaxX.size()<<endl;
    cout<<"RAMY: " << MapArcRAMCountMaxY.size()<<endl;
    cout<<"DSPX: " << MapArcDSPCountMaxX.size()<<endl;
    cout<<"DSPY: " << MapArcDSPCountMaxY.size()<<endl;
    cout<<"CLBX_C: " << MapArcCLBCountMaxX_Counter.size()<<endl;
    cout<<"CLBY_C: " << MapArcCLBCountMaxY_Counter.size()<<endl;
    cout<<"RAMX_C: " << MapArcRAMCountMaxX_Counter.size()<<endl;
    cout<<"RAMY_C: " << MapArcRAMCountMaxY_Counter.size()<<endl;
    cout<<"DSPX_C: " << MapArcDSPCountMaxX_Counter.size()<<endl;
    cout<<"DSPY_C: " << MapArcDSPCountMaxY_Counter.size()<<endl;

    cout<<"==========="<<endl;
    cout<< "Finish Getting Count of Unique X,Y Coords from Architecture"<<endl;
    cout<< "========="<<endl;

    //Finish Preping Variable, ITS TIME FOR ALGORITHM
    /*
    Used Variable:
    vector<XYNameCell> VecXYNameInstCLB;
    vector<XYNameCell> VecXYNameInstRAM;
    vector<XYNameCell> VecXYNameInstDSP;

    vector<float>ArcResourceCLBX_Unique;    vector<float>CountArcResourceCLBX_Unique;     vector<float>CountArcResourceCLBX_Unique_Counter; map<float,float>MapArcCLBCountMaxX_Counter;
    vector<float>ArcResourceCLBY_Unique;    vector<float>CountArcResourceCLBY_Unique;     vector<float>CountArcResourceCLBY_Unique_Counter; map<float,float>MapArcCLBCountMaxY_Counter;
    vector<float>ArcResourceRAMX_Unique;    vector<float>CountArcResourceRAMX_Unique;     vector<float>CountArcResourceRAMX_Unique_Counter; map<float,float>MapArcRAMCountMaxX_Counter;
    vector<float>ArcResourceRAMY_Unique;    vector<float>CountArcResourceRAMY_Unique;     vector<float>CountArcResourceRAMY_Unique_Counter; map<float,float>MapArcRAMCountMaxY_Counter;
    vector<float>ArcResourceDSPX_Unique;    vector<float>CountArcResourceDSPX_Unique;     vector<float>CountArcResourceDSPX_Unique_Counter; map<float,float>MapArcDSPCountMaxX_Counter;
    vector<float>ArcResourceDSPY_Unique;    vector<float>CountArcResourceDSPY_Unique;     vector<float>CountArcResourceDSPY_Unique_Counter; map<float,float>MapArcDSPCountMaxY_Counter;

    */
    vector<float>CounterCLBY_Usage = CountArcResourceCLBY_Unique_Counter;
    vector<float>CounterRAMY_Usage = CountArcResourceRAMY_Unique_Counter;
    vector<float>CounterDSPY_Usage = CountArcResourceDSPY_Unique_Counter;
    vector<XYNameCell> VecXYNameInstCLB_LegalY = LegalizeYVector(VecXYNameInstCLB, ArcResourceCLBY_Unique, CounterCLBY_Usage, CountArcResourceCLBY_Unique);
    vector<XYNameCell> VecXYNameInstRAM_LegalY = LegalizeYVector(VecXYNameInstRAM, ArcResourceRAMY_Unique, CounterRAMY_Usage, CountArcResourceRAMY_Unique);
    vector<XYNameCell> VecXYNameInstDSP_LegalY = LegalizeYVector(VecXYNameInstDSP, ArcResourceDSPY_Unique, CounterDSPY_Usage, CountArcResourceDSPY_Unique);

    cout<<"==========="<<endl;
    cout<< "Finish Legalize Y"<<endl;
    cout<< "========="<<endl;
    /*
    //Verify, Yes, Legalization of Y is Complete
    int testamount = 0;
    for(int i=0; i<ArcResourceCLBY_Unique.size();i++)
    {
        if(CounterCLBY_Usage[i]>0){
            cout<<ArcResourceCLBY_Unique[i] <<": "<< CounterCLBY_Usage[i] <<"|" <<CountArcResourceCLBY_Unique[i] <<endl;
            testamount +=CounterCLBY_Usage[i];
        }
    }
    cout<<testamount<<endl;
    cout<<"======="<<endl;
    for(int i=0; i<ArcResourceRAMY_Unique.size();i++)
    {
        if(CounterRAMY_Usage[i]>0)
        cout<<ArcResourceRAMY_Unique[i] <<": "<< CounterRAMY_Usage[i] <<"|" <<CountArcResourceRAMY_Unique[i] <<endl;
    }
    cout<<"======="<<endl;
    for(int i=0; i<ArcResourceDSPY_Unique.size();i++)
    {
        if(CounterDSPY_Usage[i]>0)
        cout<<ArcResourceDSPY_Unique[i] <<": "<< CounterDSPY_Usage[i] <<"|" <<CountArcResourceDSPY_Unique[i] <<endl;
    }
    */

    /*
    MapArcODName
    */

    map<float,float>MapArcCLBCountMaxY_U = MapArcCLBCountMaxY_Counter;
    map<float,float>MapArcRAMCountMaxY_U = MapArcRAMCountMaxY_Counter;
    map<float,float>MapArcDSPCountMaxY_U = MapArcDSPCountMaxY_Counter;
    vector<XYNameCell> VecXYNameInstCLB_LYLX = LegalizeXVector(VecXYNameInstCLB_LegalY, ArcResourceCLBX_Unique, MapArcCLBCountMaxY_U);
    vector<XYNameCell> VecXYNameInstRAM_LYLX = LegalizeXVector(VecXYNameInstRAM_LegalY, ArcResourceRAMX_Unique, MapArcRAMCountMaxY_U);
    vector<XYNameCell> VecXYNameInstDSP_LYLX = LegalizeXVector(VecXYNameInstDSP_LegalY, ArcResourceDSPX_Unique, MapArcDSPCountMaxY_U);

    vector<InstRes> InstanceNResCLB = PairInstanceResource(VecXYNameInstCLB_LYLX, MapArcODName);
    vector<InstRes> InstanceNResRAM = PairInstanceResource(VecXYNameInstRAM_LYLX, MapArcODName);
    vector<InstRes> InstanceNResDSP = PairInstanceResource(VecXYNameInstDSP_LYLX, MapArcODName);


    cout<<"==========="<<endl;
    cout<< "Finish Legalize X"<<endl;
    cout<< "========="<<endl;
    //for(int i=0;i<InstanceNResCLB.size(); i++)
    //{
        //cout<<InstanceNResCLB[i].NInst<<"|"<<InstanceNResCLB[i].NRes<<": "<<VecXYNameInstCLB_LYLX[i].XCoord<<" "<<VecXYNameInstCLB_LYLX[i].YCoord<<endl;
    //}



    //Write TO File
    ofstream OutputFile(InputOutputName);
    for(int i=0;i<InstanceNResCLB.size(); i++)
    {
        OutputFile<<InstanceNResCLB[i].NInst<<" "<<InstanceNResCLB[i].NRes<<endl;
    }
    for(int i=0;i<InstanceNResRAM.size(); i++)
    {
        OutputFile<<InstanceNResRAM[i].NInst<<" "<<InstanceNResRAM[i].NRes<<endl;
    }
    for(int i=0;i<InstanceNResDSP.size(); i++)
    {
        OutputFile<<InstanceNResDSP[i].NInst<<" "<<InstanceNResDSP[i].NRes<<endl;
    }
    cout<<"==========="<<endl;
    cout<< "Finish Write"<<endl;
    cout<< "========="<<endl;

}

