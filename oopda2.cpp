#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
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

        return visits.emplace_back(id, date, diagnosis, prescription),
               "✅ Visit added successfully!";
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

    // ---------- EDIT ----------
    string editPatient(int id, string newName, int newAge, string newPhone) {

        if (newAge <= 0)
            return "❌ Invalid Age!";

        if (!isValidPhone(newPhone))
            return "❌ Phone must be exactly 10 digits!";

        for (auto &p : patients) {
            if (p.id == id) {
                p.name = newName;
                p.age = newAge;
                p.phone = newPhone;
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

// ================= EXPORT =================
extern "C" {

// Register
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

// History
const char* get_history(int id) {
    static string result;
    result = hs.getHistory(id);
    return result.c_str();
}

// Frequent
const char* frequent_visitors(int n) {
    static string result;
    result = hs.frequentVisitors(n);
    return result.c_str();
}

// Monthly
const char* visits_this_month(const char* month) {
    static string result;
    result = hs.visitsThisMonth(month);
    return result.c_str();
}

// Edit
const char* edit_patient(int id, const char* name, int age, const char* phone) {
    static string result;
    result = hs.editPatient(id, name, age, phone);
    return result.c_str();
}

// Delete
const char* delete_patient(int id) {
    static string result;
    result = hs.deletePatient(id);
    return result.c_str();
}

}