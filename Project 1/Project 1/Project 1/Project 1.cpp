#include <iostream>
#include <string>
#include <vector>
using namespace std;


class k_map {
private:
    int var;
    int** map_status;
    int size_j;
    int size_i;
    int* minterms;
    int** map;
    int* string_to_array(string min,int no_of_var);
    int check_next(int number, int size_j,int size_i, string dir);
    vector<int> find_implicant_pair_of_two(int i, int j);
    void find_implicant_rectangles_4(vector<vector<int>>& v);
    void find_implicant_rectangles_8(vector<vector<int>>& v);
    vector <vector<int>> prime_imp();
    vector <vector<string>> decimal_to_binary(vector<vector<int>> v);
    vector<string> xand_prime_implicants(vector<vector<string>> b);
public:
    k_map(string minterm_string, int no_of_variables);
    void print_function();
};




int main()
{
    int number_of_var;
    cout << "Welcome to my K-map minimization program!" << endl << endl << "Please start my typing in the number of variables in your k-map (Only 2 ,3, and 4 variable maps are supported)" << endl;
    cin >> number_of_var;
    while ((number_of_var != 2) && (number_of_var != 3) && (number_of_var != 4)) //correction of inputs
    {
        cout << "Number of variables must be 2,3, or 4, please try a suppoerted number" << endl;
        cin >> number_of_var;
    }
    cout << "Now please enter your minterms seperated by comas like this example: 1,2,3,4,5" << endl;
    string min;
    cin >> min;
    k_map map(min, number_of_var); //create k-map
    cout << endl;
    map.print_function(); //print function
    cout << endl << endl;
}


int* k_map::string_to_array(string min,int no_of_var)
{
    bool acceptable = true;
    int* array;
    string num;
    int actual_num;
    array = new int[min.size()/2 + 1];
    int index = 0;
    for (int i = 0; i < min.size(); i++)
    {
        num = "";
        while ((min[i] != ',' ) && (i < min.size()))
        {
            num = num + min[i];
            i++;
        }
        actual_num = stoi(num);
        array[index] = actual_num;
        index++;
    }
    for (int i = 0; i < min.size() / 2+1; i++)
    {
        if (no_of_var == 2)
        {
            if (array[i] > 3)
                acceptable = false;
        }
        else if ((no_of_var == 3) && (array[i] > 7))
            acceptable = false;
        else if ((no_of_var == 4) && (array[i] > 15))
            acceptable = false;
            
            
            
    }
    if(acceptable == true)
        return array;
    else
    {
        string answer;
        cout << "You have entered a minterm that isn't supported by the k-map you have choosen, please retry." << endl;
        cin >> answer;
        return string_to_array(answer, no_of_var);
    }
}
k_map::k_map(string minterm_string, int no_of_variables)
{
    int* map_index;
  
    minterms = string_to_array(minterm_string,no_of_variables); //convert string of minterms to array containing every minterm
    if (no_of_variables == 2)
    {
        size_j = 2;
        size_i = 2;
        map = new int* [size_i];
        for(int i = 0; i < 2; i++)
            map[i] = new int[2];
        int arr[2] = { 0,1 };
        map_index = arr;
        var = 2;
    }
    else
    {
        if (no_of_variables == 3)
        {
            var = 3;
            size_j = 4;
            size_i = 2;
        }
        else if (no_of_variables == 4)
        {
            var = 4;
            size_j = 4;
            size_i = 4;
        }
        map = new int* [size_i];
        for (int i = 0; i < size_i; i++)
            map[i] = new int[4];
        int arr[4] = { 0,1,3,2 }; //index of array since 11 is swapped with 10 in k-map
        map_index = arr;
    }
      for(int i= 0; i < size_i; i++)
          for (int j = 0; j < size_j; j++)
          {
              map[i][j] = 0;
              for (int c = 0; c < minterm_string.size() / 2 + 1; c++)
                  if ((map_index[i] * size_j) + map_index[j] == minterms[c]) //converts coordinates of [i][j] to minterm number
                      map[i][j] = 1;
          }
      for (int i = 0; i < size_i; i++)
      {
          for (int j = 0; j < size_j; j++)
              std::cout << map[i][j] << "   "; //print k-map
          cout << endl;
      }
      map_status = new int* [size_i]; //status of k-map for creating prime implicant (0 if minterm is 0, -1 if minterm is not in a prime implicant and is 1, and 1 if it is put in a prime implicant
      for (int i = 0; i < size_j; i++)
          map_status[i] = new int[size_j];
      for (int i = 0; i < size_i; i++)
          for (int j = 0; j < size_j; j++)
          {
              if (map[i][j] == 0)
                  map_status[i][j] = 0;
              if (map[i][j] == 1)
                  map_status[i][j] = -1;
          }
   
}
void k_map::print_function()
{
    vector < vector<int> >pr = prime_imp(); // create prime implicants and stores it
  
    vector<vector<string>> bins = decimal_to_binary(pr); //convert decimal (minterm) to its binary represtantion stored as a string
    vector<string> minimized = xand_prime_implicants(bins); // xands the the contents of each essential prime implicant for minimization purposes to determine the relationship between each minterm
    cout << "F = ";
    for (int i = 0; i < minimized.size(); i++)
    {
        char c;
        int index;
        if ((size_j == 4) && (size_i == 2))
        {
            index = 3;
            if (minimized[0] == "000") //000 if all the map is covered with 1
                cout << '1';
        }
        else if ((size_j == 4) && (size_i == 4))
        {
            index = 4;
            if (minimized[0] == "0000")
                cout << '1';

        }
        else
        {
            index = 2;
            if (minimized[0] == "00") //00 if all the map is covered with 1
                cout << '1';
        }

            for (int j = 0; j < index; j++)
            {
                
                if (minimized[i][j] == '1') //if the char is 1 then there is a relationship
                {
                    c = bins[i][0][j]; //sees if both the minterms kth binary digit is 0 or 1
                    if (j == 0)
                    {
                        if (c == '0') ///if 0 then check which digit it is in and prints its corrosponding letter representation
                            cout << "A'"; 
                        else
                            cout << "A";
                    }
                    if (j == 1)
                    {
                        if (c == '0')
                            cout << "B'";
                        else
                            cout << "B";
                    }
                    if (j == 2)
                    {
                        if (c == '0')
                            cout << "C'";
                        else
                            cout << "C";
                    }
                    if (j == 3)
                    {
                        if (c == '0')
                            cout << "D'";
                        else
                            cout << "D";
                    }
                     
                }
                
            }
        if(i != minimized.size()-1)
            cout << " + ";
        
    }
    
    
    
}
void k_map::find_implicant_rectangles_8(vector<vector<int>>& v)
{
    int* map_index;
    if (size_j == 2)
    {

        int arr[2] = { 0,1 };
        map_index = arr;

    }
    else
    {
        int arr[4] = { 0,1,3,2 };
        map_index = arr;
    }

    //find part 1
    //untouched 1
    for (int i = 0; i < size_i; i++)
    {
        bool exists = true;
        for (int j = 0; j < size_j; j++)
        {
            if ((map_status[i][j] == 0) || (map_status[i][j] == 1))
                exists = false;
            if ((map_status[check_next(i, size_j, size_i, "DOWN")][j] == 0) || (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 1))
                exists = false;
        }

        if (exists == true)
        {
            
            vector<int> pr;
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[i] * size_j) + map_index[k]);
                map_status[i][k] = 1;
            }
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[k]);
                map_status[check_next(i, size_j, size_i, "DOWN")][k] = 1;
            }
            v.push_back(pr);
        }
    }
    //touched 1
    for (int i = 0; i < size_i; i++)
    {
        bool exists = true;
        for (int j = 0; j < size_j; j++)
        {
            if (map_status[i][j] == 1)
                exists = false;
            if (map_status[i][j] == 0)
                exists = false;
            if (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 0)
                exists = false;
        }
        if (exists == true)
        {
            
            vector<int> pr;
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[i] * size_j) + map_index[k]);
                map_status[i][k] = 1;
            }
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[k]);
                map_status[check_next(i, size_j, size_i, "DOWN")][k] = 1;
            }
            v.push_back(pr);
            
        }
    }
    //find part 2
        //untouched 1
    for (int j = 0; j < size_j; j++)
    {
        bool exists = true;
        for (int i = 0; i < size_i; i++)
        {
            if (map_status[i][j] == 1)
                exists = false;
            if ((map_status[i][j] == 0) || (map_status[i][j] == 1))
                exists = false;
            if ((map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 0) || (map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 1))
                exists = false;
        }

        if (exists == true)
        {
            
            vector<int> pr;
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[k] * size_j) + map_index[j]);
                map_status[k][j] = 1;
            }
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[k] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]);
                map_status[k][check_next(j, size_j, size_i, "RIGHT")] = 1;
            }
            v.push_back(pr);
        }
    }
    //touched 1
    for (int j = 0; j < size_j; j++)
    {
        bool exists = true;
        for (int i = 0; i < size_i; i++)
        {
            if (map_status[i][j] == 1)
                exists = false;
            if (map_status[i][j] == 0)
                exists = false;
            if (map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 0)
                exists = false;
        }
        if (exists == true)
        {
            
            vector<int> pr;
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[k] * size_j) + map_index[j]);
                map_status[k][j] = 1;
            }
            for (int k = 0; k < size_j; k++)
            {
                pr.push_back((map_index[k] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]);
                map_status[k][check_next(j, size_j, size_i, "RIGHT")] = 1;
            }
            v.push_back(pr);

        }
    }
        
}
vector<vector<int>> k_map::prime_imp()
{
    vector <vector<int>> prime_implicants;
    if (size_j == 2)
    {
        //check if 2^4 exists
        bool exists = true;
        for (int i = 0; i < size_i; i++)
            for (int j = 0; j < size_j; j++)
            {
                if (map[i][j] == 0)
                    exists = false;
            }
        if (exists == true)
        {
            vector<int> prime_implicant;
            prime_implicant.push_back(0);
            prime_implicant.push_back(1);
            prime_implicant.push_back(2);
            prime_implicant.push_back(3);
            prime_implicants.push_back(prime_implicant);
            return prime_implicants;
        }
        else
        {
            //check for pairs of two
            for (int i = 0; i < size_i; i++)
                for (int j = 0; j < size_j; j++)
                    if (map_status[i][j] == -1) //if it is a 1 still not included
                    {
             
                        prime_implicants.push_back(find_implicant_pair_of_two(i, j));
                    }
        }
        return prime_implicants;
    }
    else if((size_j == 4) && (size_i == 2))
    {
        //check uf 2^8 exists
        bool exists = true;
        for (int i = 0; i < size_i; i++)
            for (int j = 0; j < size_j; j++)
            {
                if (map[i][j] == 0)
                    exists = false;
            }
        if (exists == true)
        {
            vector<int> prime_implicant;
            for (int i = 0; i < 8; i++)
                prime_implicant.push_back(i);
            prime_implicants.push_back(prime_implicant);
            return prime_implicants;
        }
       
        //check if 2^4 exists
        find_implicant_rectangles_4(prime_implicants);
        //check for pairs of two
        for (int i = 0; i < size_i; i++)
            for (int j = 0; j < size_j; j++)
                if (map_status[i][j] == -1) //if it is a 1 still not included
                {

                    prime_implicants.push_back(find_implicant_pair_of_two(i, j));
                }
        return prime_implicants;
    }
    else if ((size_j == 4) && (size_i == 4))
    {
        
        //check if 2^16 exists
        bool exists = true;
        for (int i = 0; i < size_i; i++)
            for (int j = 0; j < size_j; j++)
            {
                if (map[i][j] == 0)
                    exists = false;
            }
        if (exists == true)
        {
            vector<int> prime_implicant;
            for (int i = 0; i < 16; i++)
                prime_implicant.push_back(i);
            prime_implicants.push_back(prime_implicant);
            return prime_implicants;
        }
        find_implicant_rectangles_8(prime_implicants); //check 2^8
        find_implicant_rectangles_4(prime_implicants); //checks 2^4
       //check for pairs of two
        for (int i = 0; i < size_i; i++)
            for (int j = 0; j < size_j; j++)
                if (map_status[i][j] == -1) //if it is a 1 still not included
                {

                    prime_implicants.push_back(find_implicant_pair_of_two(i, j));
                }
        return prime_implicants;

    }
}
vector<vector<string>> k_map::decimal_to_binary(vector<vector<int>> v)
{
    int temp;
    string bin;
    vector<vector<string>> answer;
    for (int i = 0; i < v.size(); i++)
    {
        
        vector<string> arr;
        for (int j = 0; j < v[i].size(); j++)
        {
           
            temp = v[i][j];
            
            bin = "";
            while(temp>0)
            {
                bin = to_string(temp % 2)+bin;
                temp = temp / 2;
            }
          
            if ((size_j == 4) && (size_i == 2))
                while (bin.size() != 3) // adds 0s to make all binary represenatations same size ex: 10 --> 010
                    bin = '0' + bin;
            else if (size_j == 2)
                while (bin.size() != 2)
                    bin = '0' + bin;
            else if ((size_j == 4) && (size_i == 4))
                while (bin.size() != 4)
                    bin = '0' + bin;
            arr.push_back(bin);
        }
        answer.push_back(arr);
    }
    return answer;
    
}
vector<string> k_map::xand_prime_implicants(vector<vector<string>> b)
{
    vector<string> answer;
    string temp;
    for (int i = 0; i < b.size(); i++)
    {
        temp = "";
        if (b[i].size() == 1) // if there is only one minterm i nthe prime implicant
            answer.push_back("1111"); //push 111 since all letters are relevant
        else
        {
            for (int k = 0; k < b[i][0].size(); k++)
            {
                bool the_same = true;
                char binary_number = b[i][0][k];
                for (int j = 0; j < b[i].size(); j++)
                {
                    if (b[i][j][k] != binary_number) //checks if all binary on the same digit are the same
                        the_same = false;

                }
                if (the_same == true) //if the same, then they are relevant (1), else, they are irrelevant(0)
                    temp = temp + '1';
                else
                    temp = temp + '0';
            }
            answer.push_back(temp);
        }
    }
    return answer;
   
}
int k_map::check_next(int number, int size_j,int size_i, string dir)
{
    int answer; //incremention of i and js to check for prime implicants
    if (size_j == 2)
    {
        if (dir == "UP")
        {
            if (number == 0)
                answer = 1;
            else
                answer = number - 1;
        }
        if (dir == "RIGHT")
        {
            if (number == 1)
                answer = 0;
            else
                answer = number + 1;
        }
        if(dir == "LEFT")
        {
            if (number == 0)
                answer = 1;
            else
                answer = number - 1;
        }
        if (dir == "DOWN")
        {
            if (number == 1)
                answer = 0;
            else
                answer = number + 1;
        }
    }
    else if ((size_j == 4) && (size_i == 2))
    {
        if (dir == "UP")
        {
            if (number == 0)
                answer = 1;
            else
                answer = number - 1;
        }
        if (dir == "DOWN")
        {
            if (number == 1)
                answer = 0;
            else
                answer = number + 1;
        }
        if (dir == "RIGHT")
        {
            if (number == 3)
                answer = 0;
            else
                answer = number + 1;
        }
        if (dir == "LEFT")
        {
            if (number == 0)
                answer = 3;
            else
                answer = number - 1;
        }
    }
    else if ((size_j == 4) && (size_i == 4))
    {
        if (dir == "UP")
        {
            if (number == 0)
                answer = 3;
            else
                answer = number - 1;
        }
        if (dir == "DOWN")
        {
            if (number == 3)
                answer = 0;
            else
                answer = number + 1;
        }
        if (dir == "RIGHT")
        {
            if (number == 3)
                answer = 0;
            else
                answer = number + 1;
        }
        if (dir == "LEFT")
        {
            if (number == 0)
                answer = 3;
            else
                answer = number - 1;
        }
    }
    return answer;
}
vector<int> k_map::find_implicant_pair_of_two(int i, int j)
{
    int* map_index;
    if (size_j == 2)
    {
       
        int arr[2] = { 0,1 };
        map_index = arr;

    }
    else
    {
        int arr[4] = { 0,1,3,2 };
        map_index = arr;
    }





    vector<int> answer;
    int minterm, second_minterm;
    //check if any 1 around not included
    //check right
    if (map_status[i][check_next(j, size_j,size_i, "RIGHT")] == -1)
    {
       
        map_status[i][j] = 1;
        map_status[i][check_next(j, size_j, size_i, "RIGHT")] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")];
        //cout << "test" << minterm <<endl;
        answer.push_back(minterm);
        answer.push_back(second_minterm);
       
        return answer;
    }
    //check left
    if (map_status[i][check_next(j, size_j, size_i, "LEFT")]== -1)
    {
        map_status[i][j] = 1;
        map_status[i][check_next(j, size_j, size_i, "LEFT")] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "LEFT")];
       
        answer.push_back(second_minterm);
        answer.push_back(minterm);
        
        return answer;
    }
    //check up
    if (map_status[check_next(i, size_j, size_i, "UP")][j] == -1)
    {
        map_status[i][j] = 1;
        map_status[check_next(i, size_j, size_i, "UP")][j] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[check_next(i, size_j, size_i, "UP")] * size_j) + map_index[j];
        
        answer.push_back(second_minterm);
        answer.push_back(minterm);
        
        return answer;
    }
    //check down
    if (map_status[check_next(i, size_j, size_i, "DOWN")][j] == -1)
    {
        map_status[i][j] = 1;
        map_status[check_next(i, size_j, size_i, "DOWN")][j] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[j];

       
        answer.push_back(minterm);
        answer.push_back(second_minterm);
        
        return answer;
    }

    //check if any 1 around already included
    
    //check right
    if (map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 1)
    {
      
        map_status[i][j] = 1;
        map_status[i][check_next(j, size_j, size_i, "RIGHT")] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")];
       
        answer.push_back(minterm);
        answer.push_back(second_minterm);
       
        return answer;
    }
    //check left
    if (map_status[i][check_next(j, size_j, size_i, "LEFT")] == 1)
    {
        map_status[i][j] = 1;
        map_status[i][check_next(j, size_j, size_i, "LEFT")]= 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "LEFT")];
       
        answer.push_back(second_minterm);
        answer.push_back(minterm);
        return answer;
    }
    //check up
    if (map_status[check_next(i, size_j, size_i, "UP")][j] == 1)
    {
        map_status[i][j] = 1;
        map_status[check_next(i, size_j, size_i, "UP")][j] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[check_next(i, size_j, size_i, "UP")] * size_j) + map_index[j];
       
        answer.push_back(second_minterm);
        answer.push_back(minterm);
        return answer;
    }
    //check down
    if (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 1)
    {
        map_status[i][j] = 1;
        map_status[check_next(i, size_j, size_i, "DOWN")][j] = 1;
        minterm = (map_index[i] * size_j) + map_index[j];
        second_minterm = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[j];
        
        answer.push_back(minterm);
        answer.push_back(second_minterm);
        
        return answer;
    }

    //isolated case
    map_status[i][j] = 1;
    minterm = (map_index[i] * size_j) + map_index[j];
    answer.push_back(minterm);
    return answer;
}
void k_map::find_implicant_rectangles_4(vector<vector<int>> &v)
{
    for (int i = 0; i < size_i; i++)
    {
        int* map_index;
        if (size_j == 2)
        {

            int arr[2] = { 0,1 };
            map_index = arr;

        }
        else
        {
            int arr[4] = { 0,1,3,2 };
            map_index = arr;
        }
        for (int i = 0; i < size_i; i++)
        {

            bool exists = true;
            if (map_status[i][0] == 1)
                exists = false;
            for (int j = 0; j < size_j; j++)
                if (map[i][j] != -1)
                    exists = false;
            if (exists == true)
            {
                vector<int> prime_implicant;
                for (int j = 0; j < size_j; j++)
                {
                    prime_implicant.push_back((map_index[i] * size_j) + map_index[j]);
                    map_status[i][j] = 1;
                }
               
              
                v.push_back(prime_implicant);
            }
        }//find with touched 1
        for (int i = 0; i < size_i; i++)
        {
            bool exists = true;
            for (int j = 0; j < size_j; j++)
                if (map[i][j] == 0)
                    exists = false;
            if ((map_status[i][0] == 1) && (map_status[i][1] == 1) && (map_status[i][2] == 1) && (map_status[i][3] == 1))
                exists = false;
            if (exists == true)
            {
                vector<int> prime_implicant;
                for (int j = 0; j < size_j; j++)
                {
                    prime_implicant.push_back((map_index[i] * size_j) + map_index[j]);
                    map_status[i][j] = 1;
                }
                
                v.push_back(prime_implicant);
            }
        }
        for (int j = 0; j < size_j; j++)
        {
            if (map[i][j] == -1) ///find with untouched 1
            {
                bool exists = true;
                if (map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 0) //check right
                    exists = false;
                if (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 0) //check down
                    exists = false;
                if (map_status[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] == 0) //check down/right
                    exists = false;

                if (map_status[i][check_next(j, size_j, size_i, "RIGHT")] == 1) //check right
                    exists = false;
                if (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 1) //check down
                    exists = false;
                if (map_status[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] == 1) //check down/right
                    exists = false;

                if (exists == true)
                {

                    map_status[i][j] = 1;
                    map_status[i][check_next(j, size_j, size_i, "RIGHT")] = 1;
                    map_status[check_next(i, size_j, size_i, "DOWN")][j] = 1;
                    map_status[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] = 1;
                    int minterm = (map_index[i] * size_j) + map_index[j]; //actual number
                    int second_min = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]; //right
                    int third_min = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[j]; //down
                    int fourth_min = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]; //down/right
                    vector<int> prime_implicant;
                    prime_implicant.push_back(minterm);
                    prime_implicant.push_back(second_min);
                    prime_implicant.push_back(third_min);
                    prime_implicant.push_back(fourth_min);
                    v.push_back(prime_implicant);
                }
            }
            //try again but with touched 1
            else if (map[i][j] == 1) ///find with touched 1
            {
                bool exists = true;
                if (map[i][check_next(j, size_j, size_i, "RIGHT")] == 0) //check right
                    exists = false;
                if (map[check_next(i, size_j, size_i, "DOWN")][j] == 0) //check down
                    exists = false;
                if (map[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] == 0) //check down/right
                    exists = false;
                if ((map_status[i][j] == 1) && (map_status[check_next(i, size_j, size_i, "DOWN")][j] == 1) && (map_status[i][check_next(j, size_j, size_i, "RIGHT")]) && (map_status[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] == 1))
                    exists = false;
                if (exists == true)
                {

                    map_status[i][j] = 1;
                    map_status[i][check_next(j, size_j, size_i, "RIGHT")] = 1;
                    map_status[check_next(i, size_j, size_i, "DOWN")][j] = 1;
                    map_status[check_next(i, size_j, size_i, "DOWN")][check_next(j, size_j, size_i, "RIGHT")] = 1;
                    int minterm = (map_index[i] * size_j) + map_index[j]; //actual number
                    int second_min = (map_index[i] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]; //right
                    int third_min = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[j]; //down
                    int fourth_min = (map_index[check_next(i, size_j, size_i, "DOWN")] * size_j) + map_index[check_next(j, size_j, size_i, "RIGHT")]; //down/right
                    vector<int> prime_implicant;
                    prime_implicant.push_back(minterm);
                    prime_implicant.push_back(second_min);
                    prime_implicant.push_back(third_min);
                    prime_implicant.push_back(fourth_min);
                    v.push_back(prime_implicant);
                }
            }
        }
    }

}
