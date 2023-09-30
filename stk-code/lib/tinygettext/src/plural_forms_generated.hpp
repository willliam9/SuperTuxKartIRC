// Generated by update_plural_forms.py, do not edit
#include <unordered_map>
std::unordered_map<std::string, tinygettext::PluralForms> g_plural_forms = {
{ "Plural-Forms:nplurals=2;plural=(n==1||n==2||n==3)||(n%10!=4||n%10!=6||n%10!=9);", tinygettext::PluralForms(2, [](int n)-> unsigned int { return (n==1||n==2||n==3)||(n%10!=4||n%10!=6||n%10!=9); }) }, // fil
{ "Plural-Forms:nplurals=4;plural=(n%1==0&&n%10==1&&n%100!=11?0:n%1==0&&n%10>=2&&n%10<=4&&(n%100<12||n%100>14)?1:n%1==0&&(n%10==0||(n%10>=5&&n%10<=9)||(n%100>=11&&n%100<=14))?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n%1==0&&n%10==1&&n%100!=11?0:n%1==0&&n%10>=2&&n%10<=4&&(n%100<12||n%100>14)?1:n%1==0&&(n%10==0||(n%10>=5&&n%10<=9)||(n%100>=11&&n%100<=14))?2:3); }) }, // uk
{ "Plural-Forms:nplurals=3;plural=n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2;", tinygettext::PluralForms(3, [](int n)-> unsigned int { return n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2; }) }, // hr
{ "Plural-Forms:nplurals=6;plural=n==0?0:n==1?1:n==2?2:n%100>=3&&n%100<=10?3:n%100>=11&&n%100<=99?4:5;", tinygettext::PluralForms(6, [](int n)-> unsigned int { return n==0?0:n==1?1:n==2?2:n%100>=3&&n%100<=10?3:n%100>=11&&n%100<=99?4:5; }) }, // ar
{ "Plural-Forms:nplurals=2;plural=(n!=1);", tinygettext::PluralForms(2, [](int n)-> unsigned int { return (n!=1); }) }, // eu
{ "Plural-Forms:nplurals=4;plural=(n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<12||n%100>14)?1:n%10==0||(n%10>=5&&n%10<=9)||(n%100>=11&&n%100<=14)?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<12||n%100>14)?1:n%10==0||(n%10>=5&&n%10<=9)||(n%100>=11&&n%100<=14)?2:3); }) }, // ru
{ "Plural-Forms:nplurals=1;plural=0;", tinygettext::PluralForms(1, [](int n)-> unsigned int { return 0; }) }, // th
{ "Plural-Forms:nplurals=3;plural=(n==1?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2);", tinygettext::PluralForms(3, [](int n)-> unsigned int { return (n==1?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2); }) }, // szl
{ "Plural-Forms:nplurals=3;plural=(n==0||n==1)?0:n!=0&&n%1000000==0?1:2;", tinygettext::PluralForms(3, [](int n)-> unsigned int { return (n==0||n==1)?0:n!=0&&n%1000000==0?1:2; }) }, // pt
{ "Plural-Forms:nplurals=5;plural=((n%10==1)&&(n%100!=11)&&(n%100!=71)&&(n%100!=91)?0:(n%10==2)&&(n%100!=12)&&(n%100!=72)&&(n%100!=92)?1:(n%10==3||n%10==4||n%10==9)&&(n%100<10||n%100>19)&&(n%100<70||n%100>79)&&(n%100<90||n%100>99)?2:(n!=0&&n%1000000==0)?3:4);", tinygettext::PluralForms(5, [](int n)-> unsigned int { return ((n%10==1)&&(n%100!=11)&&(n%100!=71)&&(n%100!=91)?0:(n%10==2)&&(n%100!=12)&&(n%100!=72)&&(n%100!=92)?1:(n%10==3||n%10==4||n%10==9)&&(n%100<10||n%100>19)&&(n%100<70||n%100>79)&&(n%100<90||n%100>99)?2:(n!=0&&n%1000000==0)?3:4); }) }, // br
{ "Plural-Forms:nplurals=3;plural=n==1?0:n!=0&&n%1000000==0?1:2;", tinygettext::PluralForms(3, [](int n)-> unsigned int { return n==1?0:n!=0&&n%1000000==0?1:2; }) }, // it
{ "Plural-Forms:nplurals=6;plural=n==0?0:n==1?1:(n%100==2||n%100==22||n%100==42||n%100==62||n%100==82)||n%1000==0&&(n%100000>=1000&&n%100000<=20000||n%100000==40000||n%100000==60000||n%100000==80000)||n!=0&&n%1000000==100000?2:(n%100==3||n%100==23||n%100==43||n%100==63||n%100==83)?3:n!=1&&(n%100==1||n%100==21||n%100==41||n%100==61||n%100==81)?4:5;", tinygettext::PluralForms(6, [](int n)-> unsigned int { return n==0?0:n==1?1:(n%100==2||n%100==22||n%100==42||n%100==62||n%100==82)||n%1000==0&&(n%100000>=1000&&n%100000<=20000||n%100000==40000||n%100000==60000||n%100000==80000)||n!=0&&n%1000000==100000?2:(n%100==3||n%100==23||n%100==43||n%100==63||n%100==83)?3:n!=1&&(n%100==1||n%100==21||n%100==41||n%100==61||n%100==81)?4:5; }) }, // kw
{ "Plural-Forms:nplurals=2;plural=(n%10==1&&n%100!=11)?0:1;", tinygettext::PluralForms(2, [](int n)-> unsigned int { return (n%10==1&&n%100!=11)?0:1; }) }, // mk
{ "Plural-Forms:nplurals=4;plural=(n==1&&n%1==0)?0:(n>=2&&n<=4&&n%1==0)?1:(n%1!=0)?2:3;", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n==1&&n%1==0)?0:(n>=2&&n<=4&&n%1==0)?1:(n%1!=0)?2:3; }) }, // cs
{ "Plural-Forms:nplurals=4;plural=(n==1&&n%1==0)?0:(n==2&&n%1==0)?1:(n%10==0&&n%1==0&&n>10)?2:3;", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n==1&&n%1==0)?0:(n==2&&n%1==0)?1:(n%10==0&&n%1==0&&n>10)?2:3; }) }, // he
{ "Plural-Forms:nplurals=2;plural=(n>1);", tinygettext::PluralForms(2, [](int n)-> unsigned int { return (n>1); }) }, // oc
{ "Plural-Forms:nplurals=2;plural=(n%10!=1||n%100==11);", tinygettext::PluralForms(2, [](int n)-> unsigned int { return (n%10!=1||n%100==11); }) }, // is
{ "Plural-Forms:nplurals=4;plural=(n==1||n==11)?0:(n==2||n==12)?1:(n>2&&n<20)?2:3;", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n==1||n==11)?0:(n==2||n==12)?1:(n>2&&n<20)?2:3; }) }, // gd
{ "Plural-Forms:nplurals=4;plural=(n%10==1&&(n%100>19||n%100<11)?0:(n%10>=2&&n%10<=9)&&(n%100>19||n%100<11)?1:n%1!=0?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n%10==1&&(n%100>19||n%100<11)?0:(n%10>=2&&n%10<=9)&&(n%100>19||n%100<11)?1:n%1!=0?2:3); }) }, // lt
{ "Plural-Forms:nplurals=3;plural=(n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2);", tinygettext::PluralForms(3, [](int n)-> unsigned int { return (n%10==1&&n%100!=11?0:n%10>=2&&n%10<=4&&(n%100<10||n%100>=20)?1:2); }) }, // bs
{ "Plural-Forms:nplurals=4;plural=(n%100==1?0:n%100==2?1:n%100==3||n%100==4?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n%100==1?0:n%100==2?1:n%100==3||n%100==4?2:3); }) }, // sl
{ "Plural-Forms:nplurals=4;plural=(n%1==0&&n==1?0:n%1==0&&n>=2&&n<=4?1:n%1!=0?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n%1==0&&n==1?0:n%1==0&&n>=2&&n<=4?1:n%1!=0?2:3); }) }, // sk
{ "Plural-Forms:nplurals=3;plural=(n%10==1&&n%100!=11?0:n!=0?1:2);", tinygettext::PluralForms(3, [](int n)-> unsigned int { return (n%10==1&&n%100!=11?0:n!=0?1:2); }) }, // lv
{ "Plural-Forms:nplurals=5;plural=(n==1?0:n==2?1:n<7?2:n<11?3:4);", tinygettext::PluralForms(5, [](int n)-> unsigned int { return (n==1?0:n==2?1:n<7?2:n<11?3:4); }) }, // ga
{ "Plural-Forms:nplurals=3;plural=(n==1?0:(((n%100>19)||((n%100==0)&&(n!=0)))?2:1));", tinygettext::PluralForms(3, [](int n)-> unsigned int { return (n==1?0:(((n%100>19)||((n%100==0)&&(n!=0)))?2:1)); }) }, // ro
{ "Plural-Forms:nplurals=4;plural=(n==1?0:(n%10>=2&&n%10<=4)&&(n%100<12||n%100>14)?1:n!=1&&(n%10>=0&&n%10<=1)||(n%10>=5&&n%10<=9)||(n%100>=12&&n%100<=14)?2:3);", tinygettext::PluralForms(4, [](int n)-> unsigned int { return (n==1?0:(n%10>=2&&n%10<=4)&&(n%100<12||n%100>14)?1:n!=1&&(n%10>=0&&n%10<=1)||(n%10>=5&&n%10<=9)||(n%100>=12&&n%100<=14)?2:3); }) }, // pl
};