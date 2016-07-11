//-------------------------------------------------------------------------/
//Copyright (C) 2003, 2004, 2005, ALL RIGHTS RESERVED.
//Centre for Sys. Eng. & App. Mech.           FEMAGSoft S.A.
//Universite Cathalique de Louvain            4, Avenue Albert Einstein
//Batiment Euler, Avenue Georges Lemaitre, 4  B-1348 Louvain-la-Neuve
//B-1348, Louvain-la-Neuve                    Belgium
//Belgium
//-------------------------------------------------------------------------/
//
//Name:         geometry.cc (all geometry premitive implementations related 
//              to polygon triangulation by sweep line algorithm)
//Author:       Liang, Wu (wu@mema.ucl.ac.be, wuliang@femagsoft.com)
//Created:      03/2001
//Modified:     10/2005. Modified and simplified only for polygon triangul-
//              ation purpose.
//-------------------------------------------------------------------------/

#if defined(WIN32)
#include <sys/timeb.h>
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <stack>
#include "utils/triangulate/Geometry.hpp"

long   int    l_id=0,p_id=0;
//Jonathan schewchuk's exact arithmetic code, see predicates.cc for detais; 
extern double orient2d(double* pa, double* pb, double* pc);

//----------------------------------------------------------------------------
//square of the distance of two points;
//----------------------------------------------------------------------------
double dist_sqr(const Pointbase& sp, const Pointbase& ep)
{
	return sqr(sp.x-ep.x)+sqr(sp.y-ep.y);
}

//----------------------------------------------------------------------------
//square of the distance of two points;
//----------------------------------------------------------------------------
double dist_sqr(double *pa, double *pb)
{
    	return sqr(pa[0]-pb[0])+sqr(pa[1]-pb[1]);
}

void UpdateKey(BTreeNode<Linebase*,double>* node, double y)
{
     node->data()->setKeyValue(y);
}

#if defined(WIN32)
void CheckTime(struct _timeb tv0, struct _timeb tv1)
{
     double runtime=1000l*(tv1.time - tv0.time)+(tv1.millitm - tv0.millitm ) / 1000l;
     cout<<"Triangulation Time::"<<runtime/1000.0<<" ( s )\n";
}
#else
void CheckTime(struct timeval tv0, struct timeval tv1)
{
     double runtime=1000l*(tv1.tv_sec - tv0.tv_sec)+(tv1.tv_usec - tv0.tv_usec) / 1000l;
     cout<<"Triangulation Time::"<<runtime/1000.0<<" ( s )\n";
}
#endif

//----------------------------------------------------------------------------
//copy constructor
//----------------------------------------------------------------------------
Pointbase::Pointbase(const Pointbase& pb)
{
        this->id=pb.id; 
	this->x=pb.x;
	this->y=pb.y;
	this->type=pb.type;
	this->left=pb.left;
}

//----------------------------------------------------------------------------
//operator ( ==, >, < and != ) overloading for pointbase class
//----------------------------------------------------------------------------
bool operator==(const Pointbase& pa, const Pointbase& pb)
{
   	return (pa.x==pb.x && pa.y==pb.y);
}

//----------------------------------------------------------------------------
bool operator>(const Pointbase& pa, const Pointbase& pb)
{
   	return( (pa.y > pb.y) || ( (pa.y==pb.y) && (pa.x < pb.x)) );
}

//----------------------------------------------------------------------------
bool operator<(const Pointbase& pa, const Pointbase& pb)
{
	return( (pa.y < pb.y) || ( (pa.y==pb.y) && (pa.x > pb.x)) );
}

//----------------------------------------------------------------------------
bool operator!=(const Pointbase& pa, const Pointbase& pb)
{
   	return !(pa.x==pb.x && pa.y==pb.y);
}

//----------------------------------------------------------------------------
//operator for debugging
//----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const  Pointbase& point)
{
    	os<<point.id<<" "<<setw(35)<<point.x<<setw(35)<<point.y<<'\n';
    	return os;
}

//----------------------------------------------------------------------------
//operator for debugging
//----------------------------------------------------------------------------
ostream &operator<<(ostream &os, const Linebase &line)
{
    	os<<"Linebase:("<<line._id<<")"<<'\n';
	os<<*(line._endp[0])<<*(line._endp[1]);
	os<<" Type="<<line._type<<" key value:"<<line.keyValue()<<'\n';
    	return os;
}

//----------------------------------------------------------------------------
//Linebase construct
//----------------------------------------------------------------------------
Linebase::Linebase():_type(UNKNOWN)
{
    	for(int i=0; i<2; i++) _endp[i]=0;
    	_id=0; 
}

//-----------------------------------------------------------------------------
//Linebase construct
//-----------------------------------------------------------------------------
Linebase::Linebase(Pointbase* sp, Pointbase* ep, Type type):_type(type)
{
    	_endp[0]=sp;
    	_endp[1]=ep;
    	//_key=_endp[0]->x < _endp[1]->x ? _endp[0]->x:_endp[1]->x;
    	_id=++l_id;
}

//----------------------------------------------------------------------------
//copy constructor
//----------------------------------------------------------------------------
Linebase::Linebase(const Linebase& line)
{
    	this->_id=line._id;
    	this->_endp[0]=line._endp[0];
    	this->_endp[1]=line._endp[1];
    	this->_key=line._key;
    	this->_helper=line._helper;
}


//----------------------------------------------------------------------------
//reverse a directed line segment, reverseable only for insert diagonals
//----------------------------------------------------------------------------
void Linebase::reverse() 
{ 
	assert(_type==INSERT); 
	Pointbase* tmp=_endp[0]; 
	_endp[0]=_endp[1];
	_endp[1]=tmp;
}

void Linebase::setKeyValue(double y)
{
        if( _endp[1]->y==_endp[0]->y )  
		_key=_endp[0]->x < _endp[1]->x ? _endp[0]->x:_endp[1]->x;
	else    _key=( y - _endp[0]->y ) * ( _endp[1]->x - _endp[0]->x ) / (_endp[1]->y - _endp[0]->y ) + _endp[0]->x; 
}

BDMFile::BDMFile(string filename, bool parse)
{ 
	//double infinity=numeric_limits<double>::max();
	const double infinity=1.0e38;
	_xmin=_ymin=infinity;
        _xmax=_ymax=-infinity;
	if(parse) read_bdm(filename);
	else read_bdm(filename, false);
	_prefix=filename;
        int pos=_prefix.find_last_of('.');
        if(pos!=-1) _prefix.erase(pos);
}
	



//----------------------------------------------------------------------------
//destructor of wrapped BDM class
////----------------------------------------------------------------------------
BDMFile::~BDMFile()
{
   	//clear all dynamic allocated memory
  	/*PointbaseMap::iterator itp=_points.begin();
   	for(; itp!=_points.end(); itp++)
   	{
        	delete itp->second;
   	}

   	LineMap::iterator itl=_edges.begin();
   	for(; itl!=_edges.end(); itl++)
   	{
         	delete itl->second;
   	}*/
	
}

//----------------------------------------------------------------------------
//read a line from input file;
//----------------------------------------------------------------------------
bool BDMFile::readline(ifstream& bdmfile, string& result)
{

bool endofile=false;

  	//Only read something that looks like a number. */
  	for(;;)
  	{
      		getline(bdmfile, result);
      		if((endofile = bdmfile.eof())) { return endofile; }
      		if( countfield(result,0x20)==0 ) continue;
      		int i=0;
      		while(result[i]==0x20) i++;
      		if( result[i]>'9' || (result[i]<'0' && result[i]!='-') ) continue;
      		else  { return (endofile = false); }
   	}
}

//----------------------------------------------------------------------------
//how many fields in string "source";
//----------------------------------------------------------------------------
int  BDMFile::countfield(const string& source, char delim)
{
  int count = 0;
  int size = (int) source.size();
  
  for(int i=0; i<size; i++) 
    {
      if(source[i]==delim) continue;
      else {
             count++;
             do{
                  i++; 
                  if( i>=size ) break;
               }  
             while( source[i]!=delim );
           }
    }

  return count; 
 
}

//----------------------------------------------------------------------------
//parse the line
//----------------------------------------------------------------------------
string BDMFile::findfield(string& source)
{
  string denstr;
  int i=0;

  //skip whitespace characters before the string
  while ( (source[i] == '\0') || ( source[i] == ' ')
          || (source[i] == '\t')) i++;

  //Skip the current field.  Stop upon reaching whitespace.
  while ((source[i] != '\0') && (source[i] != '#')
         && (source[i] != ' ') && (source[i] != '\t')) i++;

  //Now skip the whitespace and anything else that doesn't look like a
  //number, a comment, or the end of a line.
  while ((source[i] != 0x20) && (source[i] != '#')
         && (source[i] != '.') && (source[i] != '+') && (source[i] != '-')
         && ((source[i] < '0') || (source[i] > '9'))) i++;
  
  //Check for a comment (prefixed with `#').
  if (source[i] == '#') source[i] = '\0';

  denstr=source.substr(i);
  return denstr;

}

//---------------------------------------------------------------------------
//check there exists duplicated points or not;
//---------------------------------------------------------------------------
bool BDMFile::is_exist(double x, double y)
{
   bool result=false;

   PointbaseMap::iterator it=_points.begin();
   for(; it!=_points.end(); it++)
    {
      if( it->second->x==x && it->second->y==y )  { result=true; break; }
    }

   return result;
}

//----------------------------------------------------------------------------
//read boundary mesh file
//----------------------------------------------------------------------------
void BDMFile::read_bdm(const string& filename)
{

 ifstream bdmfile(filename.c_str(), ios::in);
 
 if(!bdmfile) { cout<<"File do not exist!\n"; exit(1); }

 unsigned int i = 1;
 double x,y;
 Type type;

 _ncontours=0; 


 for(;;) 
    {
      string result="\0";
      if( readline(bdmfile,result) )  break;
      _nVertices.push_back( atoi(result.c_str()) );

      if(_nVertices[_ncontours]==0) break; 
        for (unsigned int j = 0; j < _nVertices[_ncontours]; j++, i++)
        {
           if( readline(bdmfile, result) && j!=_nVertices[_ncontours] )
              {
                cout<<_nVertices[_ncontours]-j<<" vertices are missed in contour ";
                cout<<_ncontours+1<<". poly2tri stopped."<<'\n';
                bdmfile.close();
                exit(1);
               }
         
           x = (double) atof( result.c_str() );
           int nfields=countfield(result,0x20);
           if( nfields<2 )
              {
                 cout<<"Not enough data for vertex:"<<j+1<<"("<<x<<", ?)";
                 cout<<" in contour:"<<_ncontours+1<<"\npoly2tri stopped."<<'\n';
                 bdmfile.close();
                 exit(1);
              }

           string denstr=findfield(result);
           y = (double) atof( denstr.c_str() );
           type=INPUT;
 
           if( is_exist(x, y) )
              {
                 cout<<"Error! I found the duplicate point!!, the point is:";
                 cout<<i<<"("<<x<<","<<y<<").\n";
                 exit(1);
              }
           else
              {
                 Pointbase* point=new Pointbase(i,x,y,type);
		 if(x > _xmax ) _xmax=x; 
		 if(x < _xmin ) _xmin=x;
		 if(y > _ymax ) _ymax=y;
		 if(y < _ymin ) _ymin=y;
		 //point->rotate(PI/2.0);
		 _points[i]=point;
              }
         }

         _ncontours++;
     } 
   
   int sid,eid;
   int num=0;

   for(unsigned int j=0; j<_ncontours; j++)
    {
     for(i=1; i<=_nVertices[j]; i++)
       {
          sid=num+i;
          eid=(i==_nVertices[j])?num+1:num+i+1;
          type = INPUT;
          Linebase* line=new Linebase(_points[sid], _points[eid], type);
          _edges[l_id]=line;
        }
      num+=_nVertices[j];
    }

   int sum=0;
   for(unsigned int i=0; i<_ncontours; i++)
   {
	   sum+= _nVertices[i];
           _nVertices[i]=sum;
   }
	   
   p_id=num;
   bdmfile.close();
}

//----------------------------------------------------------------------------
//read boundary mesh file, without parse the input
//----------------------------------------------------------------------------
void BDMFile::read_bdm(const string& filename, bool parse)
{

 ifstream bdmfile(filename.c_str(), ios::in);
 if(!bdmfile) { cout<<"File do not exist!\n"; exit(1); }
 
 unsigned int i = 1, num;
 double x,y;
 Type type;

 _ncontours=0; 

 while(!bdmfile.eof()) 
    {
      num=0;	     
      bdmfile>>num;
      if( !bdmfile.eof() && num==0)
      {
	      cout<<"Wrong file. Don't use -p option if there are comments in your input file.\n";
              cout<<"poly2tri stopped.\n"; exit(1); 
      }
      
      if( bdmfile.eof() ) break;
      _nVertices.push_back( num );

      for (unsigned int j = 0; j < _nVertices[_ncontours]; j++, i++)
        {
           bdmfile>>x>>y;
	   if( bdmfile.eof() ) { cout<<"Unexpeced end of file! poly2tri stopped.\n"; exit(1); }
           type=INPUT;
 
           Pointbase* point=new Pointbase(i,x,y,type);
	   if(x > _xmax ) _xmax=x; 
	   if(x < _xmin ) _xmin=x;
	   if(y > _ymax ) _ymax=y;
	   if(y < _ymin ) _ymin=y;
	   //point->rotate(PI/2.0);
           _points[i]=point;
         }
         _ncontours++;
     } 
   
   int sid,eid;
   num=0;

   for(unsigned int j=0; j<_ncontours; j++)
    {
     for(i=1; i<=_nVertices[j]; i++)
       {
          sid=num+i;
          eid=(i==_nVertices[j])?num+1:num+i+1;
          type = INPUT;
          Linebase* line=new Linebase(_points[sid], _points[eid], type);
          _edges[l_id]=line;
        }
      num+=_nVertices[j];
    }

   int sum=0;
   for(unsigned int i=0; i<_ncontours; i++)
   {
	   sum+= _nVertices[i];
           _nVertices[i]=sum;
   }
	   
   p_id=num;
   bdmfile.close();
}




//----------------------------------------------------------------------------
//polygon class constructor
//----------------------------------------------------------------------------
Polygon::Polygon(string filename, bool parse):BDMFile(filename,parse)
{
   initializate();
   _debug=false;
}

//----------------------------------------------------------------------------
//polygon destructor 
//----------------------------------------------------------------------------
Polygon::~Polygon()
{
   if(_debug && _logfile.is_open() ) 
   {
           string logfile=_prefix+".log";
	   cout<<"Debug info. "<<logfile<<" was saved\n";
	   _logfile.close();
   }
   //clear all dynamic allocated memory
   PointbaseMap::iterator itp=_points.begin();
   for(; itp!=_points.end(); itp++)
   {
	   delete itp->second;
   }

   LineMap::iterator itl=_edges.begin();
   for(; itl!=_edges.end(); itl++)
   {
          delete itl->second;
   }
      
}

//----------------------------------------------------------------------------
//return the previous point (or edge) id for a given ith point (or edge);
//----------------------------------------------------------------------------
unsigned int Polygon::prev(unsigned int i)
{
   unsigned int j(0),prevLoop(0),currentLoop(0);
  
   while ( i > _nVertices[currentLoop] ) 
   {
     prevLoop=currentLoop;
     currentLoop++;     
   }
 
   if( i==1 || (i==_nVertices[prevLoop]+1) ) j=_nVertices[currentLoop];
   else if( i <= _nVertices[currentLoop] ) j=i-1;

   return j;
}

//----------------------------------------------------------------------------
//return the next point (or edge) id for a given ith point (or edge);
//----------------------------------------------------------------------------
unsigned int Polygon::next(unsigned int i)
{
   unsigned int j(0),prevLoop(0),currentLoop(0);
  
   while ( i > _nVertices[currentLoop] ) 
   {
     prevLoop=currentLoop;	   
     currentLoop++;  
   }
 
   if( i < _nVertices[currentLoop] ) j=i+1;
   else if ( i==_nVertices[currentLoop] ) 
   {
	   if( currentLoop==0) j=1;
	   else j=_nVertices[prevLoop]+1;
   }
   
   return j;
}


//----------------------------------------------------------------------------
//rotate a polygon by angle theta, reference point (0,0), not used;
//----------------------------------------------------------------------------
void Polygon::rotate(double theta)
{
    PointbaseMap::iterator it=_points.begin();	
    for(; it!=_points.end(); it++)
	    it->second->rotate(theta);
}


//----------------------------------------------------------------------------
//polygon initialization;
//to find types of all polygon vertices;
//create a priority queue for all vertices;
//construct an edge set for each vertex (the set holds all edges starting from 
//the vertex, only for loop searching purpose). 
//----------------------------------------------------------------------------
void Polygon::initializate()
{
     PointbaseMap::iterator it=_points.begin();
     for(; it!=_points.end(); it++)
	{
	    int id=it->first; 
	    int idp=prev(id);
	    int idn=next(id);
	    Pointbase p=*_points[id], pnext=*_points[idn], pprev=*_points[idp];
	   
	    if( p > pnext && pprev > p )
		   _points[id]->type=REGULAR_DOWN;
	    else if (p > pprev && pnext > p)
		   _points[id]->type=REGULAR_UP;
	    else
	    {
               double pa[2], pb[2], pc[2];
	       
               pa[0]=_points[idp]->x;
	       pa[1]=_points[idp]->y;

	       pb[0]=_points[id]->x;
	       pb[1]=_points[id]->y;

	       pc[0]=_points[idn]->x;
	       pc[1]=_points[idn]->y;
 
	       double area=orient2d(pa,pb,pc);

	       if( pprev > p && pnext > p ) _points[id]->type=(area >0) ? END: MERGE ;
	       if( pprev < p && pnext < p ) _points[id]->type=(area >0) ? START : SPLIT;
	       
	     }
	    
	    _qpoints.push(*(it->second));

	    _startAdjEdgeMap[id].insert(id);
	    
	}
}

//----------------------------------------------------------------------------
//Add a diagonal from point id i to j
//----------------------------------------------------------------------------  
void Polygon::addDiagonal(unsigned int i, unsigned int j)
{
    Type type=INSERT;
    Linebase* diag=new Linebase(_points[i], _points[j], type);
    _edges[diag->id()]=diag;
	
    _startAdjEdgeMap[i].insert(diag->id());
    _startAdjEdgeMap[j].insert(diag->id());

    _diagonals[diag->id()]=diag;     

    if(_debug) _logfile<<"Add Diagonal from "<<i<<" to "<<j<<'\n';     
}

//----------------------------------------------------------------------------
//Handle start vertex
//----------------------------------------------------------------------------
void Polygon::handleStartVertex(unsigned int i)
{
     double y=_points[i]->y;	
     _edgebst.InOrder(UpdateKey, y);
     
     _edges[i]->setHelper(i);
     _edges[i]->setKeyValue(y);      
     _edgebst.Insert(_edges[i]);

     if(_debug)
     {
     	_logfile<<"set e"<<i<<" helper to "<<i<<'\n';
     	_logfile<<"Insert e"<<i<<" to splay tree\n";
     	_logfile<<"key:"<<_edges[i]->keyValue()<<'\n';
     }
}

//----------------------------------------------------------------------------
//Handle end vertex
//----------------------------------------------------------------------------
void Polygon::handleEndVertex(unsigned int i)
{
     double y=_points[i]->y;
     _edgebst.InOrder(UpdateKey, y);
     
     unsigned int previ=prev(i);
     Linebase* edge=_edges[previ];
     unsigned int helper=_edges[previ]->helper();
    
     
     if(_points[helper]->type==MERGE) addDiagonal(i, helper);
     _edgebst.Delete(edge->keyValue());

    if(_debug)
    {
     	_logfile<<"Remove e"<<previ<<" from splay tree\n";
     	_logfile<<"key:"<<edge->keyValue()<<'\n';
    }
}

//----------------------------------------------------------------------------
//Handle split vertex
//----------------------------------------------------------------------------
void Polygon::handleSplitVertex(unsigned int i)
{
     double x=_points[i]->x, y=_points[i]->y;
     _edgebst.InOrder(UpdateKey, y);
     
     BTreeNode<Linebase*, double>*  leftnode;
     _edgebst.FindMaxSmallerThan(x, leftnode);
     Linebase* leftedge=leftnode->data();
     
     unsigned int helper=leftedge->helper();
     addDiagonal(i, helper);

     if(_debug)
     {
     	_logfile<<"Search key:"<<x<<" edge key:"<<leftedge->keyValue()<<'\n';
     	_logfile<<"e"<<leftedge->id()<<" is directly left to v"<<i<<'\n';  
     	_logfile<<"Set e"<<leftedge->id()<<" helper to "<<i<<'\n';
     	_logfile<<"set e"<<i<<" helper to "<<i<<'\n';
     	_logfile<<"Insert e"<<i<<" to splay tree\n"; 
     	_logfile<<"Insert key:"<<_edges[i]->keyValue()<<'\n';
     }
    
     leftedge->setHelper(i);
     _edges[i]->setHelper(i);
     _edges[i]->setKeyValue(y);
     _edgebst.Insert(_edges[i]);  
}


//----------------------------------------------------------------------------
//Handle merge vertex
//----------------------------------------------------------------------------
void Polygon::handleMergeVertex(unsigned int i)
{
     double x=_points[i]->x, y=_points[i]->y;
     _edgebst.InOrder(UpdateKey, y);
    
     unsigned int previ=prev(i);
     unsigned int helper=_edges[previ]->helper();
     if (_points[helper]->type==MERGE) addDiagonal(i, helper);
     _edgebst.Delete(_edges[previ]->keyValue());
     if(_debug)
     {
	     _logfile<<"e"<<previ<<" helper is "<<helper<<'\n';
	     _logfile<<"Remove e"<<previ<<" from splay tree.\n";
     }

     BTreeNode<Linebase*, double>*  leftnode;
     _edgebst.FindMaxSmallerThan(x, leftnode);
     Linebase* leftedge=leftnode->data();
            
     helper=leftedge->helper();
     if(_points[helper]->type==MERGE) addDiagonal(i, helper);
     
     leftedge->setHelper(i);

     if(_debug)
     {
     	_logfile<<"Search key:"<<x<<" found:"<<leftedge->keyValue()<<'\n';
     	_logfile<<"e"<<leftedge->id()<<" is directly left to v"<<i<<'\n';
     	_logfile<<"Set e"<<leftedge->id()<<" helper to "<<i<<'\n';
     }
}

//----------------------------------------------------------------------------
//Handle regular down vertex
//----------------------------------------------------------------------------
void Polygon::handleRegularVertexDown(unsigned int i)
{
     double y=_points[i]->y;
     _edgebst.InOrder(UpdateKey, y);
     
     unsigned int previ=prev(i);
     unsigned int helper=_edges[previ]->helper();
     if(_points[helper]->type==MERGE) addDiagonal(i, helper); 
	  
     _edgebst.Delete(_edges[previ]->keyValue());
     _edges[i]->setHelper(i);
     _edges[i]->setKeyValue(y);
     _edgebst.Insert(_edges[i]);

     if(_debug)
     {
	   _logfile<<"e"<<previ<<" helper is "<<helper<<'\n';
	   _logfile<<"Remove e"<<previ<<" from splay tree.\n";
	   _logfile<<"Set e"<<i<<" helper to "<<i<<'\n'; 
	   _logfile<<"Insert e"<<i<<" to splay tree\n"; 
	   _logfile<<"Insert key:"<<_edges[i]->keyValue()<<'\n';
     } 
}


//----------------------------------------------------------------------------
////Handle regular up vertex
//----------------------------------------------------------------------------
void Polygon::handleRegularVertexUp(unsigned int i)
{	
     double x=_points[i]->x, y=_points[i]->y;
     _edgebst.InOrder(UpdateKey, y);
     
     BTreeNode<Linebase*, double>*  leftnode;
     _edgebst.FindMaxSmallerThan(x, leftnode);
     
     Linebase* leftedge=leftnode->data();
		     
     unsigned int helper=leftedge->helper();
     if(_points[helper]->type==MERGE) addDiagonal(i, helper);
     leftedge->setHelper(i);

     if(_debug)
     {
     	_logfile<<"Search key:"<<x<<" found:"<<leftedge->keyValue()<<'\n';
     	_logfile<<"e"<<leftedge->id()<<" is directly left to v"<<i<<" and its helper is:"<<helper<<'\n';
     	_logfile<<"Set e"<<leftedge->id()<<" helper to "<<i<<'\n';
     }
}

//----------------------------------------------------------------------------
//partition polygon to monotone polygon pieces
//----------------------------------------------------------------------------
void Polygon::partition2Monotone()
{
     if(_qpoints.top().type!=START) 
     { 
		cout<<"Please check your input polygon:\n1)orientations?\n2)duplicated points?\n";
	        cout<<"poly2tri stopped.\n";
	        exit(1); 
     }
		
     while(!_qpoints.empty())
     {
          Pointbase vertex=_qpoints.top();
          _qpoints.pop();
	  unsigned int id=vertex.id;
	  
	  if(_debug)
	  {
		 string stype;
	 	 switch (vertex.type)
	  	{
	     		case START:        stype="START";       break;
			case END:          stype="END";         break;
			case MERGE:        stype="MERGE";       break;
			case SPLIT:        stype="SPLIT";       break;
			case REGULAR_UP:   stype="REGULAR_UP";  break;
			case REGULAR_DOWN: stype="REGULAR_DOWN";break;
			default: 
					   cout<<"No duplicated points please! poly2tri stopped\n";
					   exit(1); break;
		}
				    
	  	_logfile<<"\n\nHandle vertex:"<<vertex.id<<" type:"<<stype<<'\n';
	  }


    	  switch(vertex.type)
	  {
      		case START:        handleStartVertex(id);       break;
      		case END:          handleEndVertex(id);         break;
      		case MERGE:        handleMergeVertex(id);       break;
      		case SPLIT:        handleSplitVertex(id);       break;
      		case REGULAR_UP:   handleRegularVertexUp(id);   break;
		case REGULAR_DOWN: handleRegularVertexDown(id); break;
      		default: 
				   cout<<"No duplicated points please! poly2tri stopped\n";
				   exit(1); break;                         
  	   }
     }
}


//----------------------------------------------------------------------------
//two Auxiliary functions to find monotone polygon pieces
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//calculate angle B for A, B, C three given points
//----------------------------------------------------------------------------
double Polygon::angleCosb(double *pa, double *pb, double *pc)
{
  double dxab = pa[0] - pb[0];
  double dyab = pa[1] - pb[1];

  double dxcb = pc[0] - pb[0];
  double dycb = pc[1] - pb[1];

  double dxab2 = dxab * dxab;
  double dyab2 = dyab * dyab;
  double dxcb2 = dxcb * dxcb;
  double dycb2 = dycb * dycb;
  double ab = dxab2 + dyab2;
  double cb = dxcb2 + dycb2;

  double cosb = dxab * dxcb + dyab * dycb;
  double denom = sqrt( ab * cb);
  
  cosb/=denom;
  
  return cosb;
}

//----------------------------------------------------------------------------
//for any given edge, find the next edge we should choose when searching for
//monotone polygon pieces; 
//----------------------------------------------------------------------------
unsigned int Polygon::selectNextEdge(Linebase* edge)
{

    unsigned int eid= edge->endPoint(1)->id;
    set<unsigned int> edges=_startAdjEdgeMap[eid];
    assert(!edges.empty());
     
    unsigned int nexte=0;
    if( edges.size() == 1 )  nexte=*(edges.begin());
    else if( edges.size() > 1 )
    {
	unsigned int nexte_ccw(0), nexte_cw(0);
	double max=-2.0,min=2.0;
	
    	
	set<unsigned int>::iterator it=edges.begin();
	for(; it!=edges.end(); it++)
	{
		if(*it==edge->id()) continue;     
		double A[2], B[2], C[2];
		A[0]=edge->endPoint(0)->x;        A[1]=edge->endPoint(0)->y;
		B[0]=edge->endPoint(1)->x;        B[1]=edge->endPoint(1)->y;
		
		if(edge->endPoint(1)!=_edges[*it]->endPoint(0)) _edges[*it]->reverse();
		C[0]=_edges[*it]->endPoint(1)->x; C[1]=_edges[*it]->endPoint(1)->y;
		
		double area=orient2d(A, B, C);
		double cosb=angleCosb(A, B, C);

		if( area > 0 && max < cosb ) { nexte_ccw=*it; max=cosb; }
		else if( min > cosb ) { nexte_cw=*it; min=cosb; }
	}

	nexte = (nexte_ccw!=0) ? nexte_ccw : nexte_cw;
    } 

   return nexte; 
}

//----------------------------------------------------------------------------
//searching all monotone pieces;
//----------------------------------------------------------------------------
void Polygon::searchMonotones()
{
    int loop=0;
    
    LineMap edges=_edges;
    
    while( edges.size() > _diagonals.size() )
    {
	    loop++;
	    Monopoly poly;
	    LineMap::iterator it=edges.begin();
	    Pointbase* startp=it->second->endPoint(0);
	    Pointbase* endp=0;
	    Linebase*  next=it->second;
	    
	    poly.push_back(startp->id);

	    if(_debug)
	    {
	    	_logfile<<"Searching for loops:"<<loop<<'\n';
	    	_logfile<<"vertex index:"<<startp->id<<" ";
	    }
	    
	    for(;;) 
	    {
		    endp=next->endPoint(1);
		    if(next->type()!=INSERT) 
		    	{ 
				edges.erase(next->id());
				_startAdjEdgeMap[next->endPoint(0)->id].erase(next->id());
			}
		    if(endp==startp) break; 
		    poly.push_back(endp->id);
		    
		    if(_debug) _logfile<<endp->id<<" ";
		    
		    unsigned int nexte=selectNextEdge(next);
		    
		    if(nexte==0) 
		    {
			    cout<<"Please check your input polygon:\n";
		            cout<<"1)orientations?\n2)with duplicated points?\n3)is a simple one?\n";
			    cout<<"poly2tri stopped.\n";
			    exit(1);
		    }
		    //assert( nexte > 0);
		    next=edges[nexte];
		    if(next->endPoint(0) !=endp ) next->reverse(); 
	    }
	   
	    if(_debug) _logfile<<"\nloop closed!\n\n";

	    _mpolys.push_back(poly);
    }
}


//----------------------------------------------------------------------------
//triangulate a monotone polygon;
//----------------------------------------------------------------------------
void  Polygon::triangulateMonotone(Monopoly& mpoly)
{
       	
      PQueue qvertex; 	
      Monopoly::iterator it=mpoly.begin(), itnext;
      for(; itnext=it, it!=mpoly.end(); it++) 
      {
	      itnext++;  
	      if(itnext==mpoly.end()) itnext=mpoly.begin();
	      Pointbase point=*_points[*it], pointnext=*_points[*itnext];
	      point.left=(point > pointnext)? true:false;
	      qvertex.push(point);
      }
     
     stack<Pointbase> spoint;
     for(int i=0; i<2; i++) { spoint.push(qvertex.top()); qvertex.pop(); }
     
     while ( qvertex.size() > 1 )
     {
	Pointbase topQueuePoint=qvertex.top();
	Pointbase topStackPoint=spoint.top();
		
        if(topQueuePoint.left!=topStackPoint.left)
	{
            while ( spoint.size()  > 1 )
	    {
		  Pointbase p1=spoint.top();
		  spoint.pop(); 
		  Pointbase p2=spoint.top();
		  Triangle v(3);
		  v[0]=topQueuePoint.id;
		  v[1]=p1.id;
		  v[2]=p2.id;
		  _triangles.push_back(v);
		  
		  if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
		  
	    }
	    spoint.pop();
            spoint.push(topStackPoint);
            spoint.push(topQueuePoint); 	    
	}
	else
	{
	    while( spoint.size() > 1 )
	    {	    
            	Pointbase stack1Point=spoint.top();
	    	spoint.pop(); 
            	Pointbase stack2Point=spoint.top();
	    	spoint.push(stack1Point);
	    	double pa[2], pb[2], pc[2];
	    	pa[0]=topQueuePoint.x; pa[1]=topQueuePoint.y;
	    	pb[0]=stack2Point.x;   pb[1]=stack2Point.y;
	    	pc[0]=stack1Point.x;   pc[1]=stack1Point.y;
		
		if(_debug)
		{
	    		_logfile<<"current top queue vertex index="<<topQueuePoint.id<<'\n';
	    		_logfile<<"Current top stack vertex index="<<stack1Point.id<<'\n';
	    		_logfile<<"Second stack vertex index="<<stack2Point.id<<'\n';
		}

            	double area=orient2d(pa,pb,pc);
	    	bool   left=stack1Point.left;
            	if( (area > 0 && left) || (area < 0 && !left ) ) 
	    	{
                        Triangle v(3);
                        v[0]=topQueuePoint.id;
                        v[1]=stack2Point.id;
                        v[2]=stack1Point.id;
                        _triangles.push_back(v);
	                if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
	        	spoint.pop();
	    	} else break;
	    }
	    
	    spoint.push(topQueuePoint); 
	    
	}
	
	qvertex.pop();
	
     }
     
     Pointbase lastQueuePoint=qvertex.top();
     while( spoint.size() !=1 )
     {
	     Pointbase topPoint=spoint.top();
	     spoint.pop();
	     Pointbase top2Point=spoint.top();
	      
             Triangle v(3);
             v[0]=lastQueuePoint.id;
             v[1]=topPoint.id;
             v[2]=top2Point.id;
             _triangles.push_back(v);
	     
	     if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
     }
}

//----------------------------------------------------------------------------
//main triangulation function;
////----------------------------------------------------------------------------
void Polygon::triangulation()
{

#if defined(WIN32)
     struct _timeb tv0, tv1;	
     _ftime_s( &tv0);
#else
     struct timeval tv0, tv1;
     struct timezone tz;
     gettimeofday(&tv0, &tz);
#endif
					  
     partition2Monotone();
     searchMonotones();
     Monopolys::iterator it=_mpolys.begin();
     for(; it!=_mpolys.end(); it++)
	triangulateMonotone(*it);

#if defined(WIN32)
     _ftime_s( &tv1);
#else
     gettimeofday(&tv1, &tz);
#endif
     CheckTime(tv0, tv1);    
     cout<<"Total number of triangles:"<<_triangles.size()<<'\n';
		
}


void Polygon::setDebugOption(bool debug) 
{ 
     _debug=debug;
    if(_debug && !_logfile.is_open())
    {
         string logfile=_prefix+".log";
         _logfile.open(logfile.c_str(),ios::out);
    } 
       	
}

//-----------------------------------------------------------------------------
//save results: .ele for all elements and .node for all points;
//can be visualized by ShowMe.
//-----------------------------------------------------------------------------
void Polygon::saveAsShowme()
{
    string ele=_prefix+".ele";
    string node=_prefix+".node";

    fstream fele(ele.c_str(), ios::out);
    fstream fnode(node.c_str(), ios::out);

    fele<<"#ele file generated by poly2tri\n";
    fnode.precision(20);
    fnode.setf(ios::fixed, ios::floatfield);

    fnode<<"#node file generated by poly2tr\n";
    fnode<<(int)_points.size()<<'\n';
    fele<<(int)_triangles.size()<<'\n';

    PointbaseMap::iterator it=_points.begin();
    for(;it!=_points.end(); it++)
    {
	Pointbase tmp=*(it->second);
	fnode<<tmp;
    }

    long int num=0; 
    Triangles::iterator itt=_triangles.begin();
    for(; itt!=_triangles.end(); itt++)
          fele<<++num<<" "<<(*itt)[0]<<"  "<<(*itt)[1]<<"  "<<(*itt)[2]<<'\n';
    
    fnode.close();
    fele.close();

    cout<<node<<" was saved for all nodes.\n";
    cout<<ele<<" was saved for all triangles.\n";
}


//-----------------------------------------------------------------------------
//save results as TECPLOT ASCII file;
//-----------------------------------------------------------------------------
void Polygon::saveAsTecplot()
{

    string plt=_prefix+".plt";
    fstream fplt(plt.c_str(), ios::out);
    fplt<<"VARIABLES = \"X\", \"Y\" \n";
    fplt<<"ZONE N="<<_points.size()<<", E="<<_triangles.size();
    fplt<<",F=FEPOINT, ET=TRIANGLE\n";
    fplt.precision(20);
    fplt.setf(ios::fixed, ios::floatfield);

    PointbaseMap::iterator itp=_points.begin();
    for(;itp!=_points.end(); itp++)
    {
	   fplt<<itp->second->x<<"  "<<itp->second->y<<'\n';
    }

    fplt<<"\n\n\n";

    Triangles::iterator itt=_triangles.begin();
    for(; itt!=_triangles.end(); itt++)
	    fplt<<(*itt)[0]<<"  "<<(*itt)[1]<<"  "<<(*itt)[2]<<'\n';
    fplt.close();

    cout<<plt<<" was saved as TECPLOT ASCII PLT file.\n";
}


//-----------------------------------------------------------------------------
//save montone pieces/inserted diagonals in a metapost (.mp) source file; 
//Notes:1) only those diagonals partition the polygon to monotone pieces will 
//be visualized. 2)This source file can be compiled by metapost (mpost) and 
//two eps/ps files will be generated. For debug and visualization purpose only.
//-----------------------------------------------------------------------------
void Polygon::saveAsMetaPost()
{
   
    string metafile=_prefix+".mp";
    fstream fmp(metafile.c_str(), ios::out);
    
    double scalex=500.0/(_xmax-_xmin);
    double scaley=500.0/(_ymax-_ymin);
    
    double scale=(scalex <= scaley)?scalex:scaley;
    double offsetx=50-_xmin*scale,  offsety=150-_ymin*scale;

    //output all vertices;
    fmp<<"% MetaPost source code generated by poly2tri\n";
    fmp<<"pair v[];\n";
    fmp<<"path p[],t[];\n\n";

    int num;    
    PointbaseMap::iterator itp=_points.begin();
    for(;itp!=_points.end(); itp++)
    {
	    num=0;
	    Pointbase tmp=*(itp->second);
	    double x=scale*tmp.x+offsetx;
	    double y=scale*tmp.y+offsety;
	    fmp<<"v["<<itp->first<<"]=("<<x<<","<<y<<");\n";
    }
    fmp<<"\n\n";
   
    //input polygon;
    fmp<<"%EPS/PS file for input polygon.\n";
    fmp<<"beginfig(1)\n\n";
    fmp<<"for i=1 upto "<<_points.size()<<":\n";
    fmp<<"    draw fullcircle scaled 3bp shifted v[i];\n";
    fmp<<"endfor;\n\n\n";

    LineMap::iterator ite=_edges.begin();
    for(; ite!=_edges.end(); ite++)
    {
        if(ite->second->type()==INPUT)
	{		
          unsigned int sid=ite->second->endPoint(0)->id;
	  unsigned int eid=ite->second->endPoint(1)->id;
	  fmp<<" draw(v["<<sid<<"]--v["<<eid<<"]);\n";
	  //fmp<<" fill fullcircle scaled 3bp shifted v["<<sid<<"] withcolor green;\n";
	  //fmp<<" fill fullcircle scaled 3bp shifted v["<<eid<<"] withcolor green;\n";
	  //to draw all vertices' index in eps/ps file, uncomment two lines below;
	  fmp<<"%Draw two vertices index label, remove \"%\" two lines below\n"; 
	  fmp<<"%  label.top(decimal("<<sid<<"), v["<<sid<<"]);\n";
	  fmp<<"%  label.top(decimal("<<eid<<"), v["<<eid<<"]);\n";
	}
    }

    fmp<<'\n';
    fmp<<"endfig;\n\n\n"; 
       
    //set all monotone polygon paths;
    fmp<<"%EPS/PS file for all monotone polygon pieces.\n";
    fmp<<"beginfig(2)\n\n";

    int npoly=1;
    Monopolys::iterator itms=_mpolys.begin();
    for(; itms!=_mpolys.end(); itms++)
    {
	    num=1;
	    fmp<<"p["<<npoly<<"]=(";
	    Monopoly::iterator itm=itms->begin();
	    for(; itm!=itms->end(); itm++)
	    {
	    	fmp<<"v["<<*itm<<"]--"; num++;
		if(num>=10) { fmp<<"\\\n      "; num=0; }
	    }
	    fmp<<"cycle);\n\n";
	    npoly++;
    }
    
    fmp<<"\n\n";

    //draw all vertices and paths;
    fmp<<"num:="<<_mpolys.size()<<"/3;\n";
    fmp<<"for i=1 upto "<<_mpolys.size()<<":\n";
    fmp<<"    a:=uniformdeviate(num);\n";
    fmp<<"    b:=uniformdeviate(num);\n";
    fmp<<"    c:=uniformdeviate(num);\n"; 
    fmp<<"    fill p[i] withcolor (a/num, b/num, c/num);\n";
    fmp<<"    draw p[i];% withpen pencircle scaled 2pt;\n";
    fmp<<"endfor;\n\n\n";   

    fmp<<"for i=1 upto "<<_points.size()<<":\n";
    fmp<<"    draw fullcircle scaled 3bp shifted v[i];\n";
    fmp<<"endfor;\n\n\n";
    
    fmp<<"for i=1 step 1 until "<<_points.size()<<":\n";
    //to ignore all diagonal vertices' index in eps/ps file, comment two lines below;
    fmp<<"\%if you want to show all vertices' id, remove the \" % \" line below;\n";
    fmp<<"%    label.top(decimal(i), v[i]);\n";
    fmp<<"endfor;\n\n\n";

    LineMap::iterator itd=_diagonals.begin();
    for(; itd!=_diagonals.end(); itd++)
    {
        unsigned int sid=itd->second->endPoint(0)->id;
	unsigned int eid=itd->second->endPoint(1)->id;
	fmp<<" draw(v["<<sid<<"]--v["<<eid<<"]) withpen pencircle scaled 2pt withcolor red;\n";
	fmp<<" fill fullcircle scaled 3bp shifted v["<<sid<<"] withcolor green;\n";
	fmp<<" fill fullcircle scaled 3bp shifted v["<<eid<<"] withcolor green;\n";
	//to draw all vertices' index in eps/ps file, uncomment two lines below;
	fmp<<"%Draw the diagonal two vertices index label, remove \"%\" two lines below\n"; 
	fmp<<"%  label.top(decimal("<<sid<<"), v["<<sid<<"]);\n";
	fmp<<"%  label.top(decimal("<<eid<<"), v["<<eid<<"]);\n";
    }

    fmp<<'\n';
    
    fmp<<"endfig;\n\n\n"; 
   
    //draw all triangles;
    fmp<<"%EPS/PS file for triangles.\n"; 
    fmp<<"beginfig(3)\n";
    num=1;
    Triangles::iterator itt=_triangles.begin();
    for(; itt!=_triangles.end(); itt++)
    {
	fmp<<"t["<<num<<"]=(";
	fmp<<"v["<<(*itt)[0]<<"]--";
	fmp<<"v["<<(*itt)[1]<<"]--";
	fmp<<"v["<<(*itt)[2]<<"]--cycle);\n";
	num++;
    } 
   
    fmp<<'\n';
    fmp<<"for i=1 upto "<<_triangles.size()<<":\n";
    fmp<<"    draw t[i];\n";
    fmp<<"endfor;\n\n";
    fmp<<"endfig;\n\n\n";
    fmp.close();
    
    cout<<metafile<<" was saved.\n"; 
	
}

