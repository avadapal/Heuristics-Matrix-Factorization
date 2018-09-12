#include<vector>
#include<iostream>
using namespace std;

vector<double> generate_random_vec(int size){

   random_device rnd_device;
   // Specify the engine and distribution.
    mt19937 mersenne_engine(rnd_device());
    uniform_real_distribution<double> dist(0, 1);

    auto gen = std::bind(dist, mersenne_engine);
    vector<double> vec(size);
    generate(begin(vec), end(vec), gen);

        double norm = 0.0;
    for(auto a: vec){
        norm += a*a;
        }
        norm = sqrt(norm);

        for(int i = 0; i < vec.size(); i++){
                vec[i] = vec[i]/norm;

        }

return vec;

}

double squared_norm(vector<double>& vec){

    double sq_norm = 0.0;
    for(auto a: vec){
        sq_norm += a*a; 
    }

    return sq_norm;

}

vector<double> normalize(vector<double>& vec){

    double norm = 0.0;
    for(auto a: vec){
        norm += a*a; 
    }
    norm = sqrt(norm);

    for(int i = 0; i < vec.size(); i++){
        vec[i] = vec[i]/norm;

    }   

return vec;

}


vector<double> average_vectors(std::vector<double> u, std::vector<double> v){

std::vector<double> result(u.size());

    for(int  i = 0; i < u.size(); i++){
       result[i] = 0.5 * (u[i] + v[i]);
    }

    return result;
}


void print_vector(std::vector<double> v){
for(int i = 0; i < v.size(); i++){
    cout << v[i] << ", ";
}
cout << endl;
}

void print_vector(std::vector<int> v){
for(int i = 0; i < v.size(); i++){
    cout << v[i] << ", ";
}
cout << endl;
}

vector<std::string> split(std::string input, std::string delimit){
    vector<std::string> splited;
    if (input.size()==0||delimit==""){
        splited.push_back(input);
        return splited;
    }
    while (input.size()>0){
        int found=input.find(delimit);
        if(found==std::string::npos){
            splited.push_back(input);
            return splited;
        }
        else{
            splited.push_back(input.substr(0,found));
            input=input.substr(found+delimit.size(),input.size()-1);
        }
    }
    return splited;
}

double dot_product(vector<double> a1, vector<double> a2){

double answ = 0.0;

for(int i = 0; i < a1.size(); i++){

    answ += (a1[i] * a2[i]);
}

return answ;
}

bool my_comp(vector<int> x, int i, int j){

return(x[i] < x[j]);

}



double dot_product_circuit(vector<double> v, vector<double> u, double Gamma){

    double dot_product = 0.0;

    for(int  i = 0; i < v.size(); i++){
        dot_product += (v[i] * u[i]);
    }

    dot_product = (1 - dot_product) * 2 * Gamma;

    return dot_product;
}


/*std::vector<double> multiplexer(int id, std::vector<std::vector<double>> profiles){

    return profiles[id];
}*/

std::vector<double> mul_vec(std::vector<double> v, double constant){
std::vector<double> v1 = v;
    for(int i = 0; i < v.size(); i++){
        v1[i] = v1[i] * constant;
    }

    return v1;
} 

std::vector<double> sum_vectors(std::vector<double> v, std::vector<double> u){

    std::vector<double> sum_(v.size(), 0.0);
    for(int i = 0; i < v.size(); i++){
        sum_[i] = v[i] + u[i];
    }

    return sum_;
}


void intitialize_vector_zero(std::vector<double> &v){

    for(int i = 0; i < v.size(); i++){
        v[i] = 0;
    }
}

void intitialize_vector_zero(std::vector<double> &v, int profie_size){

    for(int i = 0; i < profie_size; i++){
        v[i] = 0;
    }
}