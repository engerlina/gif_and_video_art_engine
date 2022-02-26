#include "metadataFunction.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>

using namespace std;

static void generateSingleMetadata(const vector<int> &mediaDna, int currGen, const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata, const string &format)
{
    string dna = "";
    ofstream fout("./output/json/" + to_string(currGen) + ".json");
    if(fout.fail()){
        cerr << "in function 'generateSingleMetadata': error while opening in writing the file ./output/json/" + to_string(currGen) + ".json" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)mediaDna.size();++i)
        dna = dna + " " + to_string(mediaDna[i]);
        
    fout << "{" << endl;
    fout << "\t \"name\":\"" + name + " #" + to_string(currGen) + "\"," << endl;
    fout << "\t \"description\":\"" + description + "\"," << endl;
    fout << "\t \"image\":\"" + image + "/" + to_string(currGen) + "." + format + "\"," << endl;
    fout << "\t \"dna\": \"" + dna + " \"," << endl;
    fout << "\t \"edition\":\"" + to_string(currGen) + "\"," << endl;
    fout << "\t \"date\":\"" + to_string(time(NULL)) + "\"," << endl;
    fout << "\t \"attributes\": [" << endl;
    for(int i=0;i<(int)layerDir.size();++i){
        fout << "\t \t {" << endl;
        fout << "\t \t \t \"trait_type\": \"" + layerDir[i] + "\"," << endl;
        fout << "\t \t \t \"value\": \"" + singleLayer[i][mediaDna[i]] + "\"" << endl;
        if(i<(int)layerDir.size()-1) //!=
            fout << "\t \t }," << endl;
        else
            fout << "\t \t }" << endl;
    }
    fout << "\t ]," << endl;
    for(int i=0;i<(int)extraMetadata.size();++i){
        fout << "\t \"" + extraMetadata[i].first + "\": \"" + extraMetadata[i].second + "\"";
        if(i!=(int)extraMetadata.size()-1)
            fout << "," << endl;
        else
            fout << endl;
    }
    fout << "}" << endl;
    fout.close();
    return;
}

static void generateSingleSolanaMetadata(const vector<int> &mediaDna, int currGen, const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &symbol, const string &family, const string &sellerFeeBasisPoints, const string &externalUrl, const vector<string> &address, const vector<string> &share, const vector<pair<string,string>> &extraMetadata, const string &format)
{
    string dna = "", category;
    ofstream fout("./output/json/" + to_string(currGen) + ".json");
    if(fout.fail()){
        cerr << "in function 'generateSingleSolanaMetadata': error while opening in writing the file ./output/json/" + to_string(currGen) + ".json" << endl;
        exit(2);
    }
    
    if(format=="gif")
        category = "image";
    else
        category = "video";
    
    for(int i=0;i<(int)mediaDna.size();++i)
        dna = dna + " " + to_string(mediaDna[i]);
        
    fout << "{" << endl;
    fout << "\t \"name\":\"" + name + " #" + to_string(currGen+1) + "\"," << endl;
    fout << "\t \"symbol\":\"" + symbol + "\"," << endl;
    fout << "\t \"collection\":{\"name\":\"" + name + "\",\"family\":\"" + family + "\"}," << endl;
    fout << "\t \"description\":\"" + description + "\"," << endl;
    fout << "\t \"seller_fee_basis_points\":" + sellerFeeBasisPoints + "," << endl;
    fout << "\t \"image\":\"" + to_string(currGen) + "." + format + "\"," << endl;
    fout << "\t \"external_url\":\"" + externalUrl + "\"," << endl;
    fout << "\t \"dna\": \"" + dna + " \"," << endl;
    fout << "\t \"edition\":" + to_string(currGen) + "," << endl;
    fout << "\t \"date\":\"" + to_string(time(NULL)) + "\"," << endl;
    fout << "\t \"attributes\": [" << endl;
    for(int i=0;i<(int)layerDir.size();++i){
        fout << "\t \t {" << endl;
        fout << "\t \t \t \"trait_type\": \"" + layerDir[i] + "\"," << endl;
        fout << "\t \t \t \"value\": \"" + singleLayer[i][mediaDna[i]] + "\"" << endl;
        if(i<(int)layerDir.size()-1) //!=
            fout << "\t \t }," << endl;
        else
            fout << "\t \t }" << endl;
    }
    fout << "\t ]," << endl;
    fout << "\t \"properties\": {" << endl;
    fout << "\t \t \"creators\": [" << endl;
    for(int i=0;i<(int)address.size();++i){
        fout << "\t \t \t {\"address\": \"" + address[i] + "\", \"share\": " + share[i] + "}";
        if(i==(int)address.size()-1)
            fout << endl << "\t \t ]," << endl;
        else
            fout << "," << endl;
    }
    fout << "\t \t \"category\": \"" + category + "\"," << endl;
    fout << "\t \t \"files\": [{\"uri\": \"" + to_string(currGen) + "." + format +"\", \"type\": \"" + category + "/" + format + "\"}]" << endl;
    fout << "\t }," << endl; 
    
    for(int i=0;i<(int)extraMetadata.size();++i){
        fout << "\t \"" + extraMetadata[i].first + "\": \"" + extraMetadata[i].second + "\"";
        if(i!=(int)extraMetadata.size()-1)
            fout << "," << endl;
        else
            fout << endl;
    }
    fout << "}" << endl;
    fout.close();
    return;
}

void generateMetadataJson(const int &numbOfMediaToGen, const string &chain)
{
    ofstream fout("./output/json/_metadata.json");
    ifstream fin;
    string buffer;
    int i = 1, adjustForChain = 0;
    if(fout.fail()){
        cerr << "in function 'generateMetadataJson': error while opening in writing the file ./output/json/_metadata.json" << endl;
        exit(2);
    }
    
    if(chain=="SOL"){
        i = 0;
        adjustForChain = 1;
    }
    
    fout << "[" << endl;
    for(;i<=numbOfMediaToGen-adjustForChain;++i){ //da cambiare
        fin.open("./output/json/" + to_string(i) + ".json");
        if(fin.fail()){
            cerr << "in function 'generateMetadataJson': error while opening in reading the file ./output/json/" + to_string(i) + ".json" << endl;
            exit(2);
        }
        while(getline(fin, buffer))
            fout << "\t" << buffer << endl;
        if(i!=numbOfMediaToGen)
            fout << "\t ," << endl;
        else
            fout << endl;
        fin.close();
    }
    fout << "]" << endl;
    
    fout.close();
    return;
}

void generateAllMediaMetadata(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata, const vector<int> &collectionSize, const int &collIndex, vector<string> &dnaOfAllMedia, const string &format)
{
    vector<int> mediaDna((int)layerDir.size());
    for(int i=collectionSize[collIndex-1]+1;i<=collectionSize[collIndex];++i){
        extractInteger(dnaOfAllMedia[i-1], mediaDna);
        generateSingleMetadata(mediaDna, i, layerDir, singleLayer, name, description, image, extraMetadata, format);
    }
    return;
}

void generateAllMediaSolanaMetadata(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &symbol, const string &family, const string &sellerFeeBasisPoints, const string &externalUrl, const vector<string> &address, const vector<string> &share, const vector<pair<string,string>> &extraMetadata, const vector<int> &collectionSize, const int &collIndex, vector<string> &dnaOfAllMedia, const string &format)
{
    vector<int> mediaDna((int)layerDir.size());
    for(int i=collectionSize[collIndex-1];i<collectionSize[collIndex];++i){
        extractInteger(dnaOfAllMedia[i], mediaDna);
        generateSingleSolanaMetadata(mediaDna, i, layerDir, singleLayer, name, description, symbol, family, sellerFeeBasisPoints, externalUrl, address, share, extraMetadata, format);
    }
    return;
}
