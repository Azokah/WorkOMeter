#include <iostream>
#include <string>
#include <ctime> // Para la fecha y hora de las tareas
#include <cstring> // para usar el strcmp
#include <vector> // Para almacenar el vector de tareas
#include <sstream> // Para usar istringstream para transformar Strings a Int
#include <fstream> // To read and write JSON files
#include "json.hpp" // For JSON Management

using json = nlohmann::json;

class Task{
  public:
    Task(std::string, int, std::time_t);
    ~Task();

    //Setter
    void setCompletion(int);
    //Getter
    std::string getCompletionString();
    int getCompletion();
    std::string getName();
    std::string getDate();
    std::time_t getDateRaw();
    std::string toString();

  private:
    std::string name;
    int completion;
    std::time_t date;
};

Task::Task(std::string NAME, int COMPLETION, std::time_t DATE){
    name = NAME;
    completion = COMPLETION;
    date = DATE;
};

void Task::setCompletion(int x){
    completion += x;
    if(completion > 100) completion = 100;
    if(completion < 0) completion = 0;
};


std::string Task::getCompletionString(){
    std::string barra;
    barra.append("[");
    for (int p = 0; p < completion / 10; p++){
        barra.append("#");
    }
    for (int p = 10; p > completion / 10; p--){
        barra.append("-");
    }
    barra.append("]");
    return barra;
};
int Task::getCompletion(){ return completion;};
    
std::string Task::getDate() {
    //Todo este quilombo porque asctime devuelve un ENDLINE al final del string
    //y no queria que saltara al renglon de abajo
    std::string dateStr;
    dateStr = std::asctime(std::localtime(&date));
    dateStr.pop_back();
    return dateStr;
};
std::string Task::getName() { return name; };
std::time_t Task::getDateRaw(){ return date;};
std::string Task::toString(){
    //Metodo para convertir los datos de una tarea a un string y devolverlo.
    std::string str;
    str.append(getName());
    str.append(" - ");
    str.append(getCompletionString());
    str.append(". (");
    str.append(getDate());
    str.append(")");
    return str;
};

Task *newTask(std::string nameT, int completionT, std::time_t dateT){
    //Funcion que instancia una tarea y la devuelve.
    return new Task(nameT, completionT, dateT);
}

bool testArguments(int argc, char *argv[], std::vector<Task *> *vector){
    //Para poder crear tareas sin tener que ingresar a la APP. Mediante argumentos
    //Devuelve false si no habian argumentos y true si habian
    bool boolArgs = false;

    //MostrarTareas
    if(argc > 1 && std::strcmp(argv[1], "-show") == 0){
        //Muestro las tareas
        if(vector->size() == 0){
            std::cout<<"No pending tasks!"<<std::endl;
        }else {
            for (int t = 0; t < vector->size(); t++){
                std::cout << vector->at(t)->toString() << std::endl;
            }
        }
            boolArgs = true;
    }
    //Agregar una tarea

    if (argc == 4){
        if (std::strcmp(argv[1], "-add") == 0){
            int numero;
            std::string numeroStr(argv[3]); //transformar de Char* a String
            std::string nombreStr(argv[2]); //transformar de Char* a String
            std::istringstream (numeroStr) >> numero; //transformar de String a int
            vector->push_back(newTask(nombreStr,numero,std::time(NULL)));
            boolArgs = true;
        }
    }

    return boolArgs;
}

void loadTasks(std::vector<Task *> *vector){
    //Carga el json y llena el vector de tareas
    std::string name;
    int completion;
    std::time_t date;
    std::ifstream i("tasks.json");
    if (!i.is_open()) {
        std::cout<<"Failed to open json"<<std::endl;
    } else {
        json j(i);
        for (auto& element : j) {
            name = element.at("Name");
            completion = element.at("Completion");
            date = element.at("Date");
            vector->push_back(newTask(name,completion,date));
        }
    }
};
void saveTasks(std::vector<Task *> *vector){
    //Crea un JSON y lo guarda
    json j;
    for (int t = 0; t < vector->size(); t++){
           j[std::to_string(t)]["Name"] = vector->at(t)->getName();
           j[std::to_string(t)]["Completion"] = vector->at(t)->getCompletion();
           j[std::to_string(t)]["Date"] = vector->at(t)->getDateRaw();
        }
    if(j != NULL){
        std::ofstream o("tasks.json");
        o << std::setw(4) << j << std::endl;
    }
};

int main(int argc, char *argv[]){
        //Declaro las variables basicas
        int opc;
        bool running = true, choosing = true;
        std::string nameT;
        int completionT;
        std::time_t dateT;
        std::vector<Task *> tasks;
        loadTasks(&tasks);
        //Checkeo si se pusieron argumentos, si no, ingreso al menu
    if (!testArguments(argc, argv,&tasks)){

        
        std::cout << "**************************" << std::endl;
        std::cout << "* Welcome to WorkOMeter! *" << std::endl;
        std::cout << "**************************" << std::endl
                  << std::endl;

        

        //Entro al loop del menu
        while (running)
        {
            //Muestro las tareas
            for (int t = 0; t < tasks.size(); t++){
                std::cout << tasks.at(t)->toString() << std::endl;
            }
            choosing = true;
            while (choosing){
                std::cout << "Choose an option: " << std::endl;
                std::cout << "1. Create a task." << std::endl;
                std::cout << "2. Complete a task." << std::endl;
                std::cout << "3. Delete a task." << std::endl;
                std::cout << "4. Quit." << std::endl;
                std::cin >> opc;
                if (opc > 0 && opc < 5)
                    choosing = false;
            }

            switch (opc){
            case 1:
                //Agregar tarea al vector de tareas
                std::cout << "Insert the name of the task: ";
                std::cin >> nameT;
                std::cout << std::endl;
                std::cout << "Insert completion percentage (%): ";
                std::cin >> completionT;
                dateT = std::time(NULL);
                std::cout << std::endl;
                std::cout << "Date set as: " << std::asctime(std::localtime(&dateT)) << std::endl;
                tasks.push_back(newTask(nameT, completionT, dateT));
                break;
            case 2:
                //Completar Tarea
                std::cout << "Insert the name of the task(Exact name only): ";
                std::cin >> nameT;
                std::cout << std::endl;
                std::cout << "Insert completion percentage you want to add (%): ";
                std::cin >> completionT;
                for (int t = 0; t < tasks.size(); t++){
                    if(std::strcmp(nameT.c_str(),tasks.at(t)->getName().c_str()) == 0){
                        tasks.at(t)->setCompletion(completionT);
                    }
                }
                break;
            case 3:
                //Eliminar tarea del vector de tareas
                std::cout << "Insert the name of the task(Exact name only): ";
                std::cin >> nameT;
                for (int t = 0; t < tasks.size(); t++){
                    if(std::strcmp(nameT.c_str(),tasks.at(t)->getName().c_str()) == 0){
                        tasks.erase(tasks.begin()+t);
                    }
                }
                break;
            case 4:
                running = false;
                break;
            };
        };
    }
    saveTasks(&tasks);
    return 0;
};