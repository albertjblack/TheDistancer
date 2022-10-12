/*  
Jose Robles Tostado (A.K.A. Pepe Tostado || @albertjblack).
The program will read a file (cityInfo.txt) containing city information. 
This information includes city id, group, location (x, y) and city name.
We will be able to filter cities and comput the euclidean distance between two cities.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_set>
#include <iomanip>
#include <cmath>
#include <algorithm>

/* BASE DECLARATIONS */
std::string file_loc {"./cityInfo.txt"};

struct City{
  int id;
  char group;
  double longitude, latitude; 
  std::string city_name{}, city_state{};
};

/* FUNCTION HEADERS */
size_t count_entries();
City* build_entries(size_t count, std::unordered_set<char> &groups);
void trim_leading(std::string &s, std::string t);
void quick_sort(City*arr, size_t l, size_t r);
void menu(char &opt);
void display_groups(std::unordered_set<char> const &groups);
void display_cities(City *entries, size_t count);
void display_filter_cities_range(City *arr, size_t count);
void display_filter_cities_group(City const *entries, size_t const &count, std::unordered_set<char> const &groups);
void compute_distance(City city1, City city2);
int find_city_idx(City *entries, size_t count);

/* MAIN */

int main(int argc, char *argv[]){
  std::cout << "Welcome to the Distancer v2.1.3 by Pepe Tostado!";
  size_t entry_count {count_entries()};
  std::unordered_set<char> groups{};
  if (entry_count == 0){std::cout<<"Sorry, check your entries :(\n"; return 0;}
  City *entries = build_entries(entry_count, groups);
  quick_sort(entries,0,entry_count-1);
  char menu_opt;
  City city1, city2;
  city1.city_name="none";
  city2.city_name="none";
  size_t tmp_city_idx{};
  while (menu_opt != 'f'){
    menu(menu_opt);
    switch (menu_opt){
    case '0':{
      std::cout << "Valid IDs range from 1 to " << entry_count << "\n";
      display_groups(groups);
      char info_opt {'n'};
      std::cout << "Do you want to display all cities? (y/n)"<<std::endl;
      scanf(" %c", &info_opt);
      if (tolower(info_opt) == 'y'){
        display_cities(entries, entry_count);
      }
      break;
    }
    
    case 'a':{
      display_filter_cities_range(entries, entry_count);
      std::cout << "\n";
      break;
    }

    case 'b':{
      display_filter_cities_group(entries,entry_count,groups);
      break;
    }

    case 'c':{
      tmp_city_idx=find_city_idx(entries,entry_count);
      if(tmp_city_idx!=-1){
        city1=entries[tmp_city_idx];
      }
      std::cout<< "C1: " << city1.city_name<<"\n";
      break;
    }

    case 'd':{
      tmp_city_idx=find_city_idx(entries,entry_count);
      if(tmp_city_idx!=-1){
        city2=entries[tmp_city_idx];
      }
      std::cout<< "C2: " << city2.city_name <<"\n";
      break;
    }

    case 'e':{
      compute_distance(city1, city2);
      break;
    }

    default:{
      break;
    }
    }
  }

  delete [] entries;
  std::cout << "Bye now :)\nGithub.com/albertjblack";
  return 0;
}

/* FUNCTIONS */

size_t count_entries(){
  std::ifstream input_file(file_loc);
  while (!input_file.is_open()){
    std::cout << "Please check again the location of the file and update it. By default it is './cityInfo.txt'\nIf you wish to change it, type 'y' or go to the code directly.\n";
    std::cout << "Do you want to change the file: (y/n) ";
    char opt = 'n';
    std::cin >> opt;
    if (tolower(opt) == 'y'){
      std::cout << "Enter the new location for the input file: ";
      std::cin >> file_loc;
      input_file.open(file_loc);
    } else {
      std::cout << "Since you do not want to choose a different path location the program will terminate now, bye :)";
      input_file.close();
      return 0;
    }

  }
  size_t count {};
  std::string line;
  while (std::getline(input_file,line)){
    count++;
  }
  input_file.close();
  return count;
}

/* FUNCTION TO BUILD ENTRY LIST FROM FILE */
City* build_entries(size_t count, std::unordered_set<char> &groups){
  std::ifstream input_file(file_loc);
  std::string line;
  City *entries = new City[count];
  std::string id;
  for (size_t idx{};idx<count;idx++){
    std::getline(input_file,line);
    std::string tmp {};
    sscanf(line.c_str(),"%d %c %lf %lf", &entries[idx].id, &entries[idx].group, &entries[idx].longitude, &entries[idx].latitude);

    size_t i = line.find_last_of(' ');
    entries[idx].city_state = line.substr(i+1);
    tmp = line.substr(0,i);

    i = tmp.find_last_of('\t');
    entries[idx].city_name = tmp.substr(i);
    trim_leading(entries[idx].city_name," \t");
    entries[idx].city_name.erase(entries[idx].city_name.length()-1);

    if (groups.find(entries[idx].group) == groups.end()){
      groups.insert(entries[idx].group);
    }

  }
  return entries;
}

/* FX TO PROMPT THE USER TO TAKE ACTION*/
void menu(char &opt){
  char tmp_opt ='q';
  do
  {    
    std::cout << "\nChoose one of the following options: (0, a, b, c, d, e, f).\n";
    std::cout << "0) Display relevant information about the database.\n";
    std::cout << "a) Filter cities by range.\n";
    std::cout << "b) Filter cities by group.\n";
    std::cout << "c) Enter city 1\n";
    std::cout << "d) Enter city 2\n";
    std::cout << "e) Compute the euclidean distance between two cities.\n";
    std::cout << "f) Quit.\n";
    std::cin >> tmp_opt;
  } while (tmp_opt != '0' && tmp_opt != 'a' && tmp_opt != 'b'&& tmp_opt != 'c'&& tmp_opt != 'd'&& tmp_opt != 'e'&& tmp_opt != 'f');
  
  opt = tmp_opt;
}

void display_groups(std::unordered_set<char> const &groups){
  std::cout << "Groups: [";
  for (const auto &group: groups){
    std::cout << group << ",";
  }
  std::cout << "]" << std::endl;
}
void display_city(City const &entry){
    std::cout << std::left << std::setw(6) << ""
    << std::setw(6) << entry.id
    << std::setw(8) << entry.group << std::fixed << std::setprecision(2)  
    << std::setw(10) << entry.longitude << std::setprecision(2) 
    << std::setw(10) << entry.latitude 
    << std::setw(30) << entry.city_name 
    << std::setw(10) << entry.city_state << std::endl;
}

void display_cities(City *entries, size_t count){
  for(size_t idx{}; idx<count; idx++){
    std::cout << std::left << idx+1 << ") " <<entries[idx].city_name << std::endl;
  }
}

/* FUNCTION TO DISPLAY CITIES IN A RANGE */
void display_filter_cities_range(City *entries, size_t count){
  size_t lower_bound, upper_bound;
  do{
    std::cout << "Remember that the lower bound cannot be greater than the upper bound\n";
    std::cout << "Enter a lower bound in the inclusive range [1,"<<count<<"]\n";
    std::cin >> lower_bound;
    std::cout << "Enter an upper bound in the inclusive range [1,"<<count<<"]\n";
    std::cin >> upper_bound;
  } while (lower_bound < 1 || upper_bound > count || lower_bound > upper_bound);
  
  std::cout << std::endl;
  std::cout << std::fixed << std::setfill('-');
  std::cout << std::setw(190) << " \n";
  std::cout << std::setfill(' ');

  std::cout << "\n";
  while (lower_bound <= upper_bound && upper_bound <= count){
    size_t idx = lower_bound -1;
    display_city(entries[idx]);
    lower_bound++;
  }
  std::cout << std::fixed << std::setfill('-');
  std::cout << std::setw(190) << " \n";
  std::cout << std::setfill(' ');
}

/* FUNCTION TO DISPLAY CITIES IN A GROUP */
void display_filter_cities_group(City const *entries, size_t const &count, std::unordered_set<char> const &groups){
  char group {'z'};
  while(groups.find(group) == groups.end()){
    std::cout << "Enter a group from the list \n"; 
    display_groups(groups); 
    std::cin>>group;
  }
  std::cout << std::endl;
  std::cout << std::fixed << std::setfill('-');
  std::cout << std::setw(190) << " \n";
  std::cout << std::setfill(' ');
  std::cout << "\n";
  for(size_t idx{}; idx<count; idx++){
    if(entries[idx].group == group){
      display_city(entries[idx]);
    }
  }

  std::cout << std::fixed << std::setfill('-');
  std::cout << std::setw(190) << " \n";
  std::cout << std::setfill(' ');
}

/* FUNCTION TO COMPUTE ECULIDEAN DISTANCE */
void compute_distance(City city1, City city2){
  printf("City alpha: %s, City beta: %s\n", city1.city_name.c_str(), city2.city_name.c_str());;
  std::string flag {"none"};
  if(city1.city_name.compare(city2.city_name)==0){
    std::cout << "Please choose different cities. Current cities are the same or both none. Go back to menu and choose options c and d accordingly.\n";
    return;
  } else if (city1.city_name.compare(flag)==0 || city2.city_name.compare(flag)==0){
    std::cout << "Please choose different cities. One of your cities is invalid. Go back to menu and choose options c or d accordingly. \n";
    return;
  }
  double distance = sqrt(pow((city1.longitude-city2.longitude),2) + pow((city1.latitude-city2.latitude),2));
  std::cout << "Distance: " << std::fixed << std::setprecision(2)<<distance << " units squared.\n";
}

 int find_city_idx(City *entries, size_t count){
  int id{};
  do{
    std::cout << "Valid IDs range [1, "<< count << "]" << std::endl;
    std::cout << "Enter the id of the city (if previously entered, please double check. To go back enter -1"<<std::endl;
    scanf("%d",&id);
    for(size_t idx{}; idx<count;idx++){
      if(id == entries[idx].id){
        return idx;
      }
    }
  }while (id != -1);
  return -1;
}


/* FUNCTION TO REMOVE LEADING CHARACTERS */
void trim_leading(std::string &s, std::string t){
  size_t idx = s.find_first_not_of(t);
  s.erase(0,idx); 
}

/* QUICK SORT TO SORT ENTRIES BASED ON THEIR ID */
size_t partition(City *arr, size_t i, size_t j);
void quick_sort(City*arr, size_t l, size_t r){
  if(l<r){
    size_t pIdx = partition(arr,l,r);
    quick_sort(arr, l, pIdx-1);
    quick_sort(arr, pIdx+1, r);
  }
}

size_t partition(City *arr, size_t i, size_t j){
  size_t p = i;
  City tmp;
  while(i<j){
    while (i<=j && arr[i].id <=arr [p].id){
      i++;
    }
    while (arr[j].id > arr[p].id){
      j--;
    }
    if(j>i){
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }
  }
  tmp = arr[p];
  arr[p] = arr[j];
  arr[j] = tmp;
  return j;
}