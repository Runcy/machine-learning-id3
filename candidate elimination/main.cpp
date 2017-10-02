#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;
int concept=1;
//Structure instance to store each instance of the dataset
struct instance
{
    int hair;
    int feathers;
    int eggs;
    int milk;
    int airborne;
    int aquatic;
    int predator;
    int toothed;
    int backbone;
    int breathes;
    int venomous;
    int fins;
    int legs;
    int tail;
    int domestic;
    int catsize;
    int type;
};
instance temp;
// A vector of instances is created one for the data, one for general and the other for specific boundary
std::vector<instance> instances,general,specific;
template <class Container>
//A function to split a line based on the delimiter ','
void split(const std::string& str, Container& cont, char delim = ',')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}
// function to check if the second instance is consistent with the first.
int checkconsistent(instance boun,instance ins)
{
    if((boun.hair==-1||(boun.hair==ins.hair))&&(boun.feathers==-1||(boun.feathers==ins.feathers))&&(boun.eggs==-1||(boun.eggs==ins.eggs))&&(boun.milk==-1||(boun.milk==ins.milk))&&(boun.airborne==-1||(boun.airborne==ins.airborne))&&(boun.aquatic==-1||(boun.aquatic==ins.aquatic))&&(boun.predator==-1||(boun.predator==ins.predator))&&(boun.toothed==-1||(boun.toothed==ins.toothed))&&(boun.backbone==-1||(boun.backbone==ins.backbone))&&(boun.breathes==-1||(boun.breathes==ins.breathes))&&(boun.venomous==-1||(boun.venomous==ins.venomous))&&(boun.fins==-1||(boun.fins==ins.fins))&&(boun.legs==-1||(boun.legs==ins.legs))&&(boun.tail==-1||(boun.tail==ins.tail))&&(boun.domestic==-1||(boun.domestic==ins.domestic))&&(boun.catsize==-1||(boun.catsize==ins.catsize)))
       return 1;
       else
        return 0;
}
//Function to check if g1 is more general than g2
int moregeneral(instance g1,instance g2)
{
    if((g1.hair==-1&&g2.hair!=-1)||(g1.feathers==-1&&g2.feathers!=-1)||(g1.eggs==-1&&g2.eggs!=-1)||(g1.milk==-1&&g2.milk!=-1)||(g1.airborne==-1&&g2.airborne!=-1)||(g1.aquatic==-1&&g2.aquatic!=-1)||(g1.predator==-1&&g2.predator!=-1)||(g1.toothed==-1&&g2.toothed!=-1)||(g1.backbone==-1&&g2.backbone!=-1)||(g1.breathes==-1&&g2.breathes!=-1)||(g1.venomous==-1&&g2.venomous!=-1)||(g1.fins==-1&&g2.fins!=-1)||(g1.legs==-1&&g2.legs!=-1)||(g1.tail==-1&&g2.tail!=-1)||(g1.domestic==-1&&g2.domestic!=-1)||(g1.catsize==-1&&g2.catsize!=-1))
    {
      if((g1.hair!=-1&&g2.hair==-1)||(g1.feathers!=-1&&g2.feathers==-1)||(g1.eggs!=-1&&g2.eggs==-1)||(g1.milk!=-1&&g2.milk==-1)||(g1.airborne!=-1&&g2.airborne==-1)||(g1.aquatic!=-1&&g2.aquatic==-1)||(g1.predator!=-1&&g2.predator==-1)||(g1.toothed!=-1&&g2.toothed==-1)||(g1.backbone!=-1&&g2.backbone==-1)||(g1.breathes!=-1&&g2.breathes==-1)||(g1.venomous!=-1&&g2.venomous==-1)||(g1.fins!=-1&&g2.fins==-1)||(g1.legs!=-1&&g2.legs==-1)||(g1.tail!=-1&&g2.tail==-1)||(g1.domestic!=-1&&g2.domestic==-1)||(g1.catsize!=-1&&g2.catsize==-1))
    return 0;

    else
    return 1;
    }
}
//Function to check if the two insatnces are same
int same(instance genera, instance specifi)
{
    if((genera.hair==specifi.hair)&&(genera.milk==specifi.milk)&&(genera.eggs==specifi.eggs)&&(genera.airborne==specifi.airborne)&&(genera.feathers==specifi.feathers)&&(genera.aquatic==specifi.aquatic)&&(genera.predator==specifi.predator)&&(genera.toothed==specifi.toothed)&&(genera.backbone==specifi.backbone)&&(genera.breathes==specifi.breathes)&&(genera.venomous==specifi.venomous)&&(genera.fins==specifi.fins)&&(genera.legs==specifi.legs)&&(genera.tail==specifi.tail)&&(genera.domestic==specifi.domestic)&&(genera.catsize==specifi.catsize))
        return 1;
    else return 0;
}
//The main function where general and specific boundary is found
void getgeneralandspecific(int no)
{
    //initializing the general boundary to most general
    instance genb = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    general.push_back(genb);
    //initializing the specific boundary to rhe first positive test case
    for(int i=0;i<instances.size();i++)
    {
            if(instances[i].type==no)
            {
                instance speb = instances[i];
                specific.push_back(speb);
                break;
            }
    }


    for(int i=0;i<instances.size();i++)
    {
        //Updating general and specific for a positive test case
        if(instances[i].type==no)
        {
           for(int j=0;j<general.size();j++)
           {
               //removing inconsistent hypothesis from general 
               if(!checkconsistent(general[j],instances[i]))
               {
                   general.erase(general.begin()+j);
                   j--;
               }
               //Exit if entire general is inconsistent
               if(general.size()==0)
               {
                   cout<<"NOT POSSIBLE FOR CLASS "<<no<<endl<<endl;
                   return;
               }
           }
            //Updating the specific boundary to the least general approximation that holds consistent for the given test case
           if(!checkconsistent(specific[0],instances[i]))
           {
                if(specific[0].hair!=-1&&specific[0].hair!=instances[i].hair)
                    specific[0].hair=-1;
                if(specific[0].feathers!=-1&&specific[0].feathers!=instances[i].feathers)
                    specific[0].feathers=-1;
                if(specific[0].eggs!=-1&&specific[0].eggs!=instances[i].eggs)
                    specific[0].eggs=-1;
                if(specific[0].milk!=-1&&specific[0].milk!=instances[i].milk)
                    specific[0].milk=-1;
                if(specific[0].airborne!=-1&&specific[0].airborne!=instances[i].airborne)
                    specific[0].airborne=-1;
                if(specific[0].aquatic!=-1&&specific[0].aquatic!=instances[i].aquatic)
                    specific[0].aquatic=-1;
                if(specific[0].predator!=-1&&specific[0].predator!=instances[i].predator)
                    specific[0].predator=-1;
                if(specific[0].toothed!=-1&&specific[0].toothed!=instances[i].toothed)
                    specific[0].toothed=-1;
                if(specific[0].backbone!=-1&&specific[0].backbone!=instances[i].backbone)
                    specific[0].backbone=-1;
                if(specific[0].breathes!=-1&&specific[0].breathes!=instances[i].breathes)
                    specific[0].breathes=-1;
                if(specific[0].venomous!=-1&&specific[0].venomous!=instances[i].venomous)
                    specific[0].venomous=-1;
                if(specific[0].fins!=-1&&specific[0].fins!=instances[i].fins)
                    specific[0].fins=-1;
                if(specific[0].legs!=-1&&specific[0].legs!=instances[i].legs)
                    specific[0].legs=-1;
                if(specific[0].tail!=-1&&specific[0].tail!=instances[i].tail)
                    specific[0].tail=-1;
                if(specific[0].domestic!=-1&&specific[0].domestic!=instances[i].domestic)
                    specific[0].domestic=-1;
                if(specific[0].catsize!=-1&&specific[0].catsize!=instances[i].catsize)
                    specific[0].catsize=-1;

           }
            //checking if the least general case is more general than the general boundary if so it exits.
            for(int j=0;j<general.size();j++)
            {
                if(moregeneral(general[j],specific[0]))
                   {
                       ;
                   }
                else if(same(general[j],specific[0]))
                    {
                        ;
                    }
                else
                {
                   cout<<"NOT POSSIBLE FOR CLASS "<<no<<endl;
                   return;
                }

            }

        }
        else //updating the specific and general boundary for a negative case
        {
            //exits if the specific boundary is inconsistent with the case
            if(checkconsistent(specific[0],instances[i]))
            {
                   cout<<"NOT POSSIBLE FOR CLASS "<<no<<endl<<endl;
                   return;
            }
            int allcheck=0;
            //here
            // for each inconsistent hypoothesis in general boundary a least specific consistent  general boundary is added which is
            // more general than the general boundary and not more specific than the current general boundary
            int emergency=general.size();
            for(int j=0;j<emergency;j++)
            {

                if(checkconsistent(general[j],instances[i]))
                {

                                      if(general[j].hair==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.hair=specific[0].hair;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //cout<<"pushed hair\n";
                                              general.push_back(temp);
                                              }

                                      }
                                      if(general[j].milk==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.milk=specific[0].milk;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //cout<<"pushed milk\n";
                                              general.push_back(temp);
                                              }

                                      }
                                      if(general[j].eggs==-1)
                                      {
                                              instance temp=general[j];
                                              temp.eggs=specific[0].eggs;
                                              allcheck=1;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed eggs\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].airborne==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.airborne=specific[0].airborne;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed airborne\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].feathers==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.feathers=specific[0].feathers;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                              //cout<<"pushed feathers\n";
                                            }

                                      }
                                      if(general[j].aquatic==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.aquatic=specific[0].aquatic;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                              //cout<<"pushed aquatic";
                                            }

                                      }
                                      if(general[j].predator==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.predator=specific[0].predator;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                            //  cout<<"pushed predator";
                                            }

                                      }
                                      if(general[j].toothed==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.toothed=specific[0].toothed;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed tooth\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].backbone==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.backbone=specific[0].backbone;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed backbone\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].venomous==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.venomous=specific[0].venomous;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                //cout<<"i got in here";
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                              //cout<<"pushed venemous\n";
                                            }

                                      }
                                      if(general[j].breathes==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.breathes=specific[0].breathes;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed breathes\n";
                                              general.push_back(temp);
                                            }
                                      }
                                      if(general[j].fins==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.fins=specific[0].fins;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed fins\n";
                                                general.push_back(temp);
                                              }

                                      }
                                      if(general[j].legs==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.legs=specific[0].legs;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed legs\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].tail==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.tail=specific[0].tail;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              //  cout<<"pushed tail\n";
                                              general.push_back(temp);
                                            }

                                      }
                                      if(general[j].domestic==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.domestic=specific[0].domestic;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                            //  cout<<"pushed domestic\n";
                                            }

                                      }
                                      if(general[j].catsize==-1)
                                      {
                                              allcheck=1;
                                              instance temp=general[j];
                                              temp.catsize=specific[0].catsize;
                                              if(!checkconsistent(temp,instances[i])&&(moregeneral(temp,specific[0])||same(temp,specific[0])))
                                              {
                                                allcheck=0;
                                                for(int k=0;k<general.size();k++)
                                                {
                                                    if(!checkconsistent(general[k],instances[i]))
                                                    {
                                                      if(moregeneral(general[k],temp)||same(general[k],temp))
                                                      allcheck=1;

                                                    }

                                                }
                                              }
                                              if(allcheck==0)
                                              {
                                              general.push_back(temp);
                                            //  cout<<"pushed catsize\n";
                                            }

                                      }
                                      general.erase(general.begin()+j);
                                      j--;
                                      emergency--;
                }

            }

                //exits if no least specific general hypothesis exists that is consistent with the test case
                if(general.size()==0)
                {
                   cout<<"NOT POSSIBLE FOR CLASS "<<no<<endl<<endl;
                   return;
                }
                //here

            }
        }
    //prints general boundary
    cout<<"*****General boundary*****\n";
    for(int i=0;i<general.size();i++)
    {

        cout<<"<";
        if(general[i].hair==-1)
        cout<<"?,";
        else
        cout<<general[i].hair<<",";
        if(general[i].feathers==-1)
        cout<<"?,";
        else
        cout<<general[i].feathers<<",";
        if(general[i].eggs==-1)
        cout<<"?,";
        else
        cout<<general[i].eggs<<",";
        if(general[i].milk==-1)
        cout<<"?,";
        else
        cout<<general[i].milk<<",";
        if(general[i].airborne==-1)
        cout<<"?,";
        else
        cout<<general[i].airborne<<",";
        if(general[i].aquatic==-1)
        cout<<"?,";
        else
        cout<<general[i].aquatic<<",";
        if(general[i].predator==-1)
        cout<<"?,";
        else
        cout<<general[i].predator<<",";
        if(general[i].toothed==-1)
        cout<<"?,";
        else
        cout<<general[i].toothed<<",";
        if(general[i].backbone==-1)
        cout<<"?,";
        else
        cout<<general[i].backbone<<",";
        if(general[i].breathes==-1)
        cout<<"?,";
        else
        cout<<general[i].breathes<<",";
        if(general[i].venomous==-1)
        cout<<"?,";
        else
        cout<<general[i].venomous<<",";
        if(general[i].fins==-1)
        cout<<"?,";
        else
        cout<<general[i].fins<<",";
        if(general[i].legs==-1)
        cout<<"?,";
        else
        cout<<general[i].legs<<",";
        if(general[i].tail==-1)
        cout<<"?,";
        else
        cout<<general[i].tail<<",";
        if(general[i].domestic==-1)
        cout<<"?,";
        else
        cout<<general[i].domestic<<",";
        if(general[i].catsize==-1)
        cout<<"?";
        else
        cout<<general[i].catsize;
        cout<<"> ";

    }
    cout<<endl;
    //prints specific boundary
    for(int i=0;i<specific.size();i++)
    {
        cout<<"*****Specific boundary*****\n";
        cout<<"<";
        if(specific[i].hair==-1)
        cout<<"?,";
        else
        cout<<specific[i].hair<<",";
        if(specific[i].feathers==-1)
        cout<<"?,";
        else
        cout<<specific[i].feathers<<",";
        if(specific[i].eggs==-1)
        cout<<"?,";
        else
        cout<<specific[i].eggs<<",";
        if(specific[i].milk==-1)
        cout<<"?,";
        else
        cout<<specific[i].milk<<",";
        if(specific[i].airborne==-1)
        cout<<"?,";
        else
        cout<<specific[i].airborne<<",";
        if(specific[i].aquatic==-1)
        cout<<"?,";
        else
        cout<<specific[i].aquatic<<",";
        if(specific[i].predator==-1)
        cout<<"?,";
        else
        cout<<specific[i].predator<<",";
        if(specific[i].toothed==-1)
        cout<<"?,";
        else
        cout<<specific[i].toothed<<",";
        if(specific[i].backbone==-1)
        cout<<"?,";
        else
        cout<<specific[i].backbone<<",";
        if(specific[i].breathes==-1)
        cout<<"?,";
        else
        cout<<specific[i].breathes<<",";
        if(specific[i].venomous==-1)
        cout<<"?,";
        else
        cout<<specific[i].venomous<<",";
        if(specific[i].fins==-1)
        cout<<"?,";
        else
        cout<<specific[i].fins<<",";
        if(specific[i].legs==-1)
        cout<<"?,";
        else
        cout<<specific[i].legs<<",";
        if(specific[i].tail==-1)
        cout<<"?,";
        else
        cout<<specific[i].tail<<",";
        if(specific[i].domestic==-1)
        cout<<"?,";
        else
        cout<<specific[i].domestic<<",";
        if(specific[i].catsize==-1)
        cout<<"?";
        else
        cout<<specific[i].catsize;
        cout<<"> ";

    }
    cout<<endl<<endl;
}
//function to store the data in the text file to a vector of structures and detect how many training concepts
void detectandstore()
{
    ifstream fin;

    int value;
    std::string tmp;
    fin.open("data.txt",ios::in);
    if(!fin)
    {
        cerr<<"Failed to open data file\n";
        exit(1);
    }


    while(1)
    {
        if(fin.eof()) break;
        getline(fin,tmp);
        std::vector<std::string> words;
        split(tmp, words);
        for(int i=0;i<words.size();i++)
        {
            switch(i)
            {
                case 1: temp.hair=stoi(words[i]);
                        break;
                case 2: temp.feathers=stoi(words[i]);
                        break;
                case 3: temp.eggs=stoi(words[i]);
                        break;
                case 4: temp.milk=stoi(words[i]);
                        break;
                case 5: temp.airborne=stoi(words[i]);
                        break;
                case 6: temp.aquatic=stoi(words[i]);
                        break;
                case 7: temp.predator=stoi(words[i]);
                        break;
                case 8: temp.toothed=stoi(words[i]);
                        break;
                case 9: temp.backbone=stoi(words[i]);
                        break;
                case 10: temp.breathes=stoi(words[i]);
                        break;
                case 11: temp.venomous=stoi(words[i]);
                        break;
                case 12: temp.fins=stoi(words[i]);
                        break;
                case 13: temp.legs=stoi(words[i]);
                        break;
                case 14: temp.tail=stoi(words[i]);
                        break;
                case 15: temp.domestic=stoi(words[i]);
                        break;
                case 16: temp.catsize=stoi(words[i]);
                        break;
                case 17: temp.type=stoi(words[i]);
                        if (temp.type>concept)
                            concept=temp.type;
                        break;
                }

        }
        instances.push_back(temp);
    }
    fin.close();
    cout<<"******DATA ACQUIRED****** \nNumber of training concepts is:" <<concept<<endl;
}
//main function to call the detectgeneraand specific function for each target concept
int main()
{
    int finalcounter=0;
    detectandstore();
    for(int no=1;no<=concept;no++)
    {
        finalcounter=0;
        cout<<"Training for class: "<<no<<endl;
        general.clear();
        specific.clear();
        getgeneralandspecific(no);
    }
    return 0;
}
