#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
using namespace std;

// ================= PATIENT =================
class Patient {
public:
    int id;
    string name;
    int age;
    string phone;

    Patient() {}
    Patient(int i, string n, int a, string p) {
        id = i;
        name = n;
        age = a;
        phone = p;
    }
};

// ================= VISIT =================
class Visit {
public:
    int patientID;
    string date;
    string diagnosis;
    string prescription;

    Visit() {}
    Visit(int id, string d, string diag, string pres) {
        patientID = id;
        date = d;
        diagnosis = diag;
        prescription = pres;
    }
};

// ================= SYSTEM =================
class HospitalSystem {
private:
    vector<Patient> patients;
    vector<Visit> visits;

public:

    // ---------- VALIDATION ----------
    bool isValidPhone(string phone) {
        if (phone.length() != 10) return false;
        for (char c : phone) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    bool patientExists(int id) {
        for (auto &p : patients) {
            if (p.id == id) return true;
        }
        return false;
    }

    // ---------- REGISTER ----------
    string registerPatient(int id, string name, int age, string phone) {

        if (id <= 0)
            return "❌ Invalid ID!";

        if (age <= 0)
            return "❌ Invalid Age!";

        if (!isValidPhone(phone))
            return "❌ Phone must be exactly 10 digits!";

        if (patientExists(id))
            return "❌ Patient ID already exists!";

        patients.push_back(Patient(id, name, age, phone));
        return "✅ Patient registered successfully!";
    }

    // ---------- ADD VISIT ----------
    string addVisit(int id, string date, string diagnosis, string prescription) {
        if (!patientExists(id))
            return "❌ Patient does not exist!";

        visits.push_back(Visit(id, date, diagnosis, prescription));
        return "✅ Visit added successfully!";
    }

    // ---------- HISTORY ----------
    string getHistory(int id) {
        string result = "";

        bool found = false;
        for (auto &p : patients) {
            if (p.id == id) {
                result += "Patient: " + p.name +
                          " | Age: " + to_string(p.age) + "\n";
                found = true;
                break;
            }
        }

        if (!found)
            return "❌ Patient not found!";

        result += "Visit History:\n";

        for (auto &v : visits) {
            if (v.patientID == id) {
                result += v.date + " | " +
                          v.diagnosis + " | " +
                          v.prescription + "\n";
            }
        }

        return result;
    }

    // ---------- FREQUENT ----------
    string frequentVisitors(int N) {
        string result = "Frequent Visitors:\n";

        for (auto &p : patients) {
            int count = 0;

            for (auto &v : visits) {
                if (v.patientID == p.id)
                    count++;
            }

            if (count > N) {
                result += p.name + " (" +
                          to_string(count) + " visits)\n";
            }
        }

        return result;
    }

    // ---------- MONTH ----------
    string visitsThisMonth(string month) {
        int count = 0;

        for (auto &v : visits) {
            if (v.date.length() >= 5 && v.date.substr(3,2) == month)
                count++;
        }

        return "Total visits this month: " + to_string(count);
    }

    // ---------- EDIT (PARTIAL UPDATE) ----------
    string editPatient(int id, string newName, int newAge, string newPhone) {

        for (auto &p : patients) {
            if (p.id == id) {

                if (newName != "")
                    p.name = newName;

                if (newAge > 0)
                    p.age = newAge;

                if (newPhone != "") {
                    if (!isValidPhone(newPhone))
                        return "❌ Invalid phone (must be 10 digits)";
                    p.phone = newPhone;
                }

                return "✅ Patient updated successfully!";
            }
        }

        return "❌ Patient not found!";
    }

    // ---------- DELETE ----------
    string deletePatient(int id) {
        bool found = false;

        for (auto it = patients.begin(); it != patients.end(); ++it) {
            if (it->id == id) {
                patients.erase(it);
                found = true;
                break;
            }
        }

        if (!found)
            return "❌ Patient not found!";

        visits.erase(
            remove_if(visits.begin(), visits.end(),
                [id](Visit &v) {
                    return v.patientID == id;
                }),
            visits.end()
        );

        return "✅ Patient and visits deleted!";
    }
};

// ================= GLOBAL =================
HospitalSystem hs;

// ================= STRING FIX =================
char* toCString(string str) {
    char* res = new char[str.size() + 1];
    strcpy(res, str.c_str());
    return res;
}

// ================= EXPORT =================
extern "C" {

char* register_patient(int id, const char* name, int age, const char* phone) {
    return toCString(hs.registerPatient(id, name, age, phone));
}

char* add_visit(int id, const char* date, const char* diagnosis, const char* prescription) {
    return toCString(hs.addVisit(id, date, diagnosis, prescription));
}

char* get_history(int id) {
    return toCString(hs.getHistory(id));
}

char* frequent_visitors(int n) {
    return toCString(hs.frequentVisitors(n));
}

char* visits_this_month(const char* month) {
    return toCString(hs.visitsThisMonth(month));
}

char* edit_patient(int id, const char* name, int age, const char* phone) {
    return toCString(hs.editPatient(id, name, age, phone));
}

char* delete_patient(int id) {
    return toCString(hs.deletePatient(id));
}

}