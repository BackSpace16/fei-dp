#include <unordered_map>
#include <string>

struct ElementData {
    std::string color;
    float radius;
};

const std::unordered_map<std::string, ElementData> elementData = {
    { "H", {"#fff", 1.20} }, // 3 odmocnina protonoveho cisla
    { "He", {"#d9ffff", 1.40} },

    { "Li", {"#cc80ff", 1.82} },
    { "Be", {"#c2ff00", 1.53} },
    { "B", {"#ffb5b5", 1.92} },
    { "C", {"#909090", 1.70} },
    { "N", {"#3050f8", 1.55} },
    { "O", {"#ff0d0d", 1.52} },
    { "F", {"#90e050", 1.47} },
    { "Ne", {"#b3e3f5", 1.54} },

    { "Na", {"#ab5cf2", 2.27} },
    { "Mg", {"#8aff00", 1.73} },
    { "Al", {"#bfa6a6", 1.84} },
    { "Si", {"#f0c8a0", 2.10} },
    { "P", {"#ff8000", 1.80} },
    { "S", {"#ffff30", 1.80} },
    { "Cl", {"#1ff01f", 1.75} },
    { "Ar", {"#80d1e3", 1.88} },

    { "K", {"#8f40d4", 2.75} },
    { "Ca", {"#3dff00", 2.31} },
    { "Sc", {"#e6e6e6", 2.11} },
    { "Ti", {"#bfc2c7", 2.46} },
    { "V", {"#a6a6ab", 1.00} },
    { "Cr", {"#8a99c7", 1.00} },
    { "Mn", {"#9c7ac7", 1.00} },
    { "Fe", {"#e06633", 1.00} },
    { "Co", {"#f090a0", 1.00} },
    { "Ni", {"#50d050", 1.63} },
    { "Cu", {"#c88033", 1.40} },
    { "Zn", {"#7d80b0", 1.39} },
    { "Ga", {"#c28f8f", 1.87} },
    { "Ge", {"#668f8f", 2.11} },
    { "As", {"#bd80e3", 1.85} },
    { "Se", {"#ffa100", 1.90} },
    { "Br", {"#a62929", 1.85} },
    { "Kr", {"#5cb8d1", 2.02} },

    { "Rb", {"#702eb0", 3.03} },
    { "Sr", {"#00ff00", 2.49} },
    { "Y", {"#94ffff", 1.00} },
    { "Zr", {"#94e0e0", 1.00} },
    { "Nb", {"#73c2c9", 1.00} },
    { "Mo", {"#54b5b5", 1.00} },
    { "Tc", {"#3b9e9e", 1.00} },
    { "Ru", {"#248f8f", 1.00} },
    { "Rh", {"#0a7d8c", 1.00} },
    { "Pd", {"#006985", 1.63} },
    { "Ag", {"#c0c0c0", 1.72} },
    { "Cd", {"#ffd98f", 1.58} },
    { "In", {"#a67573", 1.93} },
    { "Sn", {"#668080", 2.17} },
    { "Sb", {"#9e63b5", 2.06} },
    { "Te", {"#d47a00", 2.06} },
    { "I", {"#940094", 1.98} },
    { "Xe", {"#429eb0", 2.16} },

    { "Cs", {"#57178f", 3.43} },
    { "Ba", {"#00c900", 2.68} },

    { "La", {"#70d4ff", 1.00} },
    { "Ce", {"#ffffc7", 1.00} },
    { "Pr", {"#d9ffc7", 1.00} },
    { "Nd", {"#c7ffc7", 1.00} },
    { "Pm", {"#a3ffc7", 1.00} },
    { "Sm", {"#8fffc7", 1.00} },
    { "Eu", {"#61ffc7", 1.00} },
    { "Gd", {"#45ffc7", 1.00} },
    { "Tb", {"#30ffc7", 1.00} },
    { "Dy", {"#1fffc7", 1.00} },
    { "Ho", {"#00ff9c", 1.00} },
    { "Er", {"#00e675", 1.00} },
    { "Tm", {"#00d452", 1.00} },
    { "Yb", {"#00bf38", 1.00} },

    { "Lu", {"#00ab24", 1.00} },
    { "Hf", {"#4dc2ff", 1.00} },
    { "Ta", {"#4da6ff", 1.00} },
    { "W", {"#2194d6", 1.00} },
    { "Re", {"#267dab", 1.00} },
    { "Os", {"#266696", 1.00} },
    { "Ir", {"#175487", 1.00} },
    { "Pt", {"#d0d0e0", 1.75} },
    { "Au", {"#ffd123", 1.66} },
    { "Hg", {"#b8b8d0", 1.55} },
    { "Tl", {"#a6544d", 1.96} },
    { "Pb", {"#575961", 2.02} },
    { "Bi", {"#9e4fb5", 2.07} },
    { "Po", {"#ab5c00", 1.97} },
    { "At", {"#754f45", 2.02} },
    { "Rn", {"#428296", 2.20} },

    { "Fr", {"#420066", 3.48} },
    { "Ra", {"#007d00", 2.83} },

    { "Ac", {"#70abfa", 1.00} },
    { "Th", {"#00baff", 1.00} },
    { "Pa", {"#00a1ff", 1.00} },
    { "U", {"#008fff", 1.86} },
    { "Np", {"#0080ff", 1.00} },
    { "Pu", {"#006bff", 1.00} },
    { "Am", {"#545cf2", 1.00} },
    { "Cm", {"#785ce3", 1.00} },
    { "Bk", {"#8a4fe3", 1.00} },
    { "Cf", {"#a136d4", 1.00} },
    { "Es", {"#b31fd4", 1.00} },
    { "Fm", {"#b31fba", 1.00} },
    { "Md", {"#b30da6", 1.00} },
    { "No", {"#bd0d87", 1.00} },

    { "Lr", {"#c70066", 1.00} },
    { "Rf", {"#cc0059", 1.00} },
    { "Db", {"#d1004f", 1.00} },
    { "Sg", {"#d90045", 1.00} },
    { "Bh", {"#e00038", 1.00} },
    { "Hs", {"#e6002e", 1.00} },
    { "Mt", {"#eb0026", 1.00} }
};
