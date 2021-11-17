// Pulkov_Zachita_LAB2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <vector>
#include <algorithm>

using namespace std;

int compl_index(vector<unsigned char>v)
{
	vector<unsigned char> vv = v;
	vector<unsigned char>::iterator k;
	sort(vv.begin(), vv.end());
	k = unique(vv.begin(), vv.begin() + vv.size());
	vv.resize(distance(vv.begin(), k));

	vector<vector<int>> count;
	count.assign(25, vector<int>(vv.size()));
	vector<int> group_length(25);
	for (unsigned int key_length = 1; key_length <= 25; key_length++)
	{
		for (unsigned int i = 0; i < vv.size(); i++)
		{
			count[key_length - 1][i] = 0;
			group_length[key_length - 1] = 0;
			for (unsigned int j = 0; j < v.size(); j++)
			{
				if (j % key_length == 0)
				{
					group_length[key_length - 1]++;
				}
				if ((j % key_length == 0) & (v[j] == vv[i]))
				{
					count[key_length - 1][i]++;
				}
			}
		}
	}
	vector<double> sum;
	vector<double> ci;
	sum.resize(25);
	ci.resize(25);
	for (int i = 1; i <= 25; i++)
	{
		sum[i - 1] = 0;
		{
			for (int p = 0; p <= vv.size() - 1; p++)
			{
				sum[i - 1] = sum[i - 1] + ((double)count[i - 1][p] * ((double)count[i - 1][p] - 1.0));
			}
		}
		ci[i - 1] = 0;
		ci[i - 1] = sum[i - 1] / ((double)group_length[i - 1] * ((double)group_length[i - 1] - 1.0));
	}
	vector<double>ci_sort = ci;
	int key_length = 0;
	for (int i = 1; i <= 25; i++)
	{
		cout << i-1 <<". ";
		cout << ci_sort[i - 1] << endl;

	}
	cout << "Введите номер первого индекса наиболее отличающегося от других индексов:" << endl;
	for (int i = 1; i <= 25; i++)
	{
		cin >> i;
		i = i + 1;
		cout << "Индекс соответствия под этим номером: "<< ci_sort[i - 1] << endl;
		key_length = i;
		break;
	}
	
	return key_length;
}

char decode_key(unsigned char cSym, unsigned  char cCode)
{
	if (cCode >= cSym)
		return cCode - cSym;
	else
		return cCode + 256 - cSym;
}
void Decode(const string& Key, const string& input_file, const string& output_file)
{
	int j = 0;
	ifstream input;
	ofstream output;
	output.open(output_file + ".txt", ios::binary);
	input.open(input_file + ".txt", ios::binary);
	while (!input.eof())
	{
		char ch = input.get();
		if (j == Key.length())
		{
			j = 0;
		}
		char o;
		o = (ch - Key[j] + 256);
		output << o;
		j++;
		input.peek();
	}
}

void SymbolsCount(vector<unsigned char>& vect, int ascii[256])
{
	for (int i = 0; i < 256; i++)
		ascii[i] = 0;
	for (auto symbolic : vect)
	{
		ascii[int(symbolic)] += 1;
	}
}


char PopularSymbolic(int symbolic_count[256])
{
	unsigned char popularsymbolic;
	int max = 0;
	for (int i = 0; i < 256; i++)
		if (int(symbolic_count[i]) > max)
		{
			popularsymbolic = i;
			max = symbolic_count[i];
		}
	return popularsymbolic;
}

int main()
{
	vector<unsigned char> ModelText;
	vector<unsigned char> EncodeText;
	string decryption_text, model_text;
	string Key;
    setlocale(LC_ALL, "Russian");

	while (true)
	{
		int ans;
		cout << "Имя файла закодированного текста : " << endl;
		cin >> decryption_text;
		cout << "Имя файла эталонного текста : " << endl;
		cin >> model_text;
		int asciiModel[256];
		int asciiCode[256];
		int asciiDecode[256];
		ifstream inCode(decryption_text + ".txt", ios::binary);
		ifstream inModel(model_text + ".txt", ios::binary);

		vector<unsigned char> ModelText((istreambuf_iterator<char>(inModel)), istreambuf_iterator<char>());
		vector<unsigned char> EncodeText((istreambuf_iterator<char>(inCode)), istreambuf_iterator<char>());


		cout << "Индекс соответствия:" << endl;
		int index = compl_index(EncodeText);

		SymbolsCount(ModelText, asciiModel);
		unsigned char popular_text_char = PopularSymbolic(asciiModel);
		cout << "Самый часто встречающийся символ в эталонном тексте: '" << popular_text_char <<"'"<< endl;

		cout << index << " - значный ключ " << endl;

		vector<vector<unsigned char> > vectorCodePart(index);
		vector<unsigned char>::iterator it = EncodeText.begin();

		while (it != EncodeText.end())
		{
			for (auto& Part : vectorCodePart)
				if (it != EncodeText.end())
				{
					Part.push_back(*it);
					it++;
				}
		}

		vector<unsigned char> vectorDecode;
		for (auto vectorKey : vectorCodePart)
		{
			SymbolsCount(vectorKey, asciiDecode);
			unsigned char cPopCode = PopularSymbolic(asciiDecode);
			unsigned char cKey = decode_key(popular_text_char, cPopCode);
			Key += cKey;
		}
		cout << "Предпологаемый ключ:" << Key << endl;
		string Out;
		cout << "Имя файла для сохранения:" << endl;
		cin >> Out;
		Decode(Key, decryption_text, Out);
		cout << "Файл успешно сохранён." << endl;

	}
}


