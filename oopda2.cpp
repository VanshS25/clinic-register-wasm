#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ================= PATIENT CLASS =================
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

// ================= VISIT CLASS =================
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

// ================= HOSPITAL SYSTEM =================
class HospitalSystem {
private:
    vector<Patient> patients;
    vector<Visit> visits;

public:

    // ---------- CHECK PATIENT ----------
    bool patientExists(int id) {
        for (auto &p : patients) {
            if (p.id == id) return true;
        }
        return false;
    }

    // ---------- REGISTER PATIENT ----------
    string registerPatient(int id, string name, int age, string phone) {
        if (patientExists(id)) {
            return "❌ Patient ID already exists!";
        }

        patients.push_back(Patient(id, name, age, phone));
        return "✅ Patient registered successfully!";
    }

    // ---------- ADD VISIT ----------
    string addVisit(int id, string date, string diagnosis, string prescription) {
        if (!patientExists(id)) {
            return "❌ Patient does not exist!";
        }

        visits.push_back(Visit(id, date, diagnosis, prescription));
        return "✅ Visit added successfully!";
    }

    // ---------- VIEW HISTORY ----------
    string getHistory(int id) {
        string result = "";

        bool found = false;
        for (auto &p : patients) {
            if (p.id == id) {
                result += "Patient: " + p.name + " | Age: " + to_string(p.age) + "\n";
                found = true;
                break;
            }
        }

        if (!found) {
            return "❌ Patient not found!";
        }

        result += "Visit History:\n";

        for (auto &v : visits) {
            if (v.patientID == id) {
                result += v.date + " | " + v.diagnosis + " | " + v.prescription + "\n";
            }
        }

        return result;
    }

    // ---------- FREQUENT VISITORS ----------
    string frequentVisitors(int N) {
        string result = "Frequent Visitors:\n";

        for (auto &p : patients) {
            int count = 0;

            for (auto &v : visits) {
                if (v.patientID == p.id)
                    count++;
            }

            if (count > N) {
                result += p.name + " (" + to_string(count) + " visits)\n";
            }
        }

        return result;
    }

    // ---------- VISITS THIS MONTH ----------
    string visitsThisMonth(string month) {
        int count = 0;

        for (auto &v : visits) {
            if (v.date.substr(3,2) == month) { // DD-MM-YYYY
                count++;
            }
        }

        return "Total visits this month: " + to_string(count);
    }
};

// ================= GLOBAL OBJECT =================
HospitalSystem hs;

// ================= EXPORT FUNCTIONS =================
extern "C" {

// Register Patient
const char* register_patient(int id, const char* name, int age, const char* phone) {
    static string result;
    result = hs.registerPatient(id, name, age, phone);
    return result.c_str();
}

// Add Visit
const char* add_visit(int id, const char* date, const char* diagnosis, const char* prescription) {
    static string result;
    result = hs.addVisit(id, date, diagnosis, prescription);
    return result.c_str();
}

// Get History
const char* get_history(int id) {
    static string result;
    result = hs.getHistory(id);
    return result.c_str();
}

// Frequent Visitors
const char* frequent_visitors(int n) {
    static string result;
    result = hs.frequentVisitors(n);
    return result.c_str();
}

// Visits This Month
const char* visits_this_month(const char* month) {
    static string result;
    result = hs.visitsThisMonth(month);
    return result.c_str();
}

}