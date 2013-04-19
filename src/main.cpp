#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <list>
//#include <cstdlib>

#include "FullBitStringTable.h"
#include "FullInverseTable.h"
#include "FullCompositionTable.h"
#include "LanguageModel.h"
#include "Constraint.h"
#include "ConstraintNetwork.h"
#include "ConnectComponent.h"
#include "WeakConnectComponent.h"
#include "ALConstraintGraph.h"
#include "Bridge.h"
#include "util.h"
#include "MultiCSPFile.h"

//#define GENERATE
//#define SEGMENT
//#define LOG 
//#define DBG

using namespace std;

list<ALConstraintGraph> iterative_segment( ALConstraintGraph *alg, LanguageModel *l , int threshold ) ;
vector<ALConstraintGraph> segment( ALConstraintGraph *alg, LanguageModel *l, int threshold ) ;

void proc_param( int , char **);

FullBitStringTable fbst( 8,"../rcc8/rcc8.comp"  );
FullInverseTable fit( &fbst, "../rcc8/rcc8.conv");
FullCompositionTable fct ( &fbst, "../rcc8/rcc8.comp" );

LanguageModel lm( &fct, &fit, &fbst);

char *cp ;
char *mp ;




int
main(int argc, char *argv[]) {
#if 0
	ConstraintNetwork cn( "./test/test.csp", "./test/test.map", &lm );
	ALConstraintGraph alg( &cn , &lm);
	WeakConnectComponent wcc(&alg, &lm, lm.getConstraint(1));
	cout << "components number: " << wcc.getCount()  << endl;
	wcc.exportToFile( );
	//brg.delBridges();
	//if( brg.bridgeIsDeleted() ) 
		//alg.exportToFile("f3deleted.csp", "n3deleted.map");
#endif
#if 1
	ConstraintNetwork cn( "./newcsp/fma.csp", "./newcsp/fma.map", &lm);
	ALConstraintGraph alg( &cn, &lm);
	alg.exportToFile( "./newcsp/fma1.csp" , "./newcsp/fma1.map" ); 
    list<ALConstraintGraph> graphs = iterative_segment( &alg, &lm, 1000 );
    MultiCSPFile mcf( &graphs );
	mcf.exportToFile("fmasegmented.csp");
    
#endif

#if 0
	int V = 0;
	int E = 0;
	int ve_rate = 3;

	string prefix = "./r/random";
	string csp = ".csp";
	string map = ".map";

	cout << "V" << " " << "E" << " " << "i_th" << " " << "components" << endl; 
	for( int w = 5; w <= 6 ; w++ ) {
		V = w * 10000;
	//for( int i = 1; i <= 6 ; i++ ) {
		//E = V/2;
		E = V*ve_rate;
		int j = 1;
		while( j <= 20 ) {
			string no1 = itoa( V, 10 );
			string star = "*";
			string no2 = itoa(E , 10);
			string line = "_";
			string no3 = itoa( j , 10 );
			string middle = no1 + star + no2 + line + no3;
#ifdef GENERATE
			cout << V << " " << E << " " << j << endl; 
			ConstraintNetwork cn( V, E ,&lm );
			cn.exportToFile(prefix+middle+csp, prefix+middle+map); 
#endif


#ifdef SEGMENT
#ifndef GENERATE
		    ConstraintNetwork cn( (prefix+middle+csp).c_str(), (prefix+middle+map).c_str(), &lm);
#endif
            ALConstraintGraph alg( &cn, &lm);
            list<ALConstraintGraph> graphs = iterative_segment( &alg, &lm, 1000 );
			
			MultiCSPFile mcf( &graphs );
			mcf.exportToFile((prefix+middle+string("segmented")+csp).c_str());
#if 0
            list<ALConstraintGraph>::iterator it = graphs.begin();
            int k = 1;
            while(it!=graphs.end() ) {
                string component_no = itoa( k, 10);
                it->exportToFile(prefix+middle+component_no+csp, prefix+middle+component_no+map);
                
#ifdef LOG
                cout << "graphs[" << k << "].size() == " << it->getVexnum() << endl;
#endif
                it++;
                k++;
            }
#ifdef LOG
            cout << "totally :" << graphs.size() << " subgraphs " << endl;
#endif
            
#endif
#endif
            j++;
		}
	//}
	}
#endif
}

list<ALConstraintGraph>
iterative_segment( ALConstraintGraph *alg, LanguageModel *l , int threshold ) {
    stack<ALConstraintGraph> graph_stack;
    list<ALConstraintGraph> graphs;
    graph_stack.push( *alg );
    while( !graph_stack.empty() ) {
        
#ifdef DBG
    cout << "stack size: " << graph_stack.size() << endl;
#endif
        ALConstraintGraph gptr = graph_stack.top();
        graph_stack.pop();
#ifdef DBG
    cout << "stack size: " << graph_stack.size() << endl;
#endif
#ifdef DBG
    cout << gptr.getVexnum() << endl;
#endif
        vector<ALConstraintGraph> sresults = segment( &gptr, l, threshold );
        if( sresults.size() == 1 ) {
            graphs.push_back( gptr );
        } else {
            for(int i =0 ; i < sresults.size(); i++) {
                ALConstraintGraph r = sresults[i];
                if( r.getVexnum() > threshold ) {
                    graph_stack.push( r );
                } else {
                    graphs.push_back( r );
                }
            }
        }
    }
#ifdef DBG
    cout << "iterative_segment finished: " << endl;
#endif
    return graphs;
}

vector<ALConstraintGraph>
segment( ALConstraintGraph *alg, LanguageModel *l, int threshold ) {

#ifdef DBG
    cout << "segment start: " << alg->getVexnum()  << endl;
#endif
	Bridge brg( alg );
#ifdef DBG
    cout << "bridge num: " <<brg.getBridgeNum() << endl;
#endif
    brg.delBridges();
    WeakConnectComponent dc( alg , l , l->getConstraint(1));
#ifdef DBG
    cout << "dc component num: " << dc.getCount() << endl;
#endif
    vector<ALConstraintGraph> dc_graphs = dc.getAllComponents();
    vector<ALConstraintGraph> graphs ;
    
    for( int i = 0; i < dc_graphs.size() ; i++ ) {
        ALConstraintGraph g = dc_graphs[i];
        if(g.getVexnum() > threshold ) {
            WeakConnectComponent po( &g , l , l->getConstraint(4));
            vector<ALConstraintGraph> po_graphs = po.getAllComponents();
            for( int j = 0; j < po_graphs.size(); j++ ){
                graphs.push_back(po_graphs[j]);
            }
        }
        else {
            graphs.push_back(g );
        }
    }
#ifdef DBG
            cout << "graphs size: " << graphs.size() << endl;
#endif
    return graphs;
}

#if 0
void
proc_param( int argc , char *argv[] ) {
/*
 * four parameter styles
 *	-m csp_path map_path : check multi definition
 *	-i csp_path map_path :  check inverse error
 *	-sn csp_path map_path : segmentation by relation n
 *	-d csp_path map_path : delete isolate vertex
 *	-e csp_path map_path sets_path translattion_item_path : source error by sets file
 *	-rn csp_path map_path new_csp_path new_map_path: remove the node erasable
 *	-p csp_path map_path new_csp_path new_map_path: preprocess the model
 */
	const char *csp_path ;
	const char *map_path ;
	cp = argv[2];
	mp = argv[3];
	ALRCCModel a( cp, mp, &lm );
	al_model = &a;


	char p1 = argv[1][1];
	switch( p1)  {
	case 'm' : {
				al_model->check_multi_csp();
				break;
			   }
	case 'i' : {
				al_model->check_inverse_error();
				break;
			   }
	case 's' : {
				char *p2 = argv[1] + 2;
				int relation = atoi( p2, 10 );
				segmentation( relation, cp, mp);
				break;
			   }
	case 'd' : {
				delete_isolate_vex( cp, mp);
				break; 
			   }
	case 'e' : {
				AssertionBase ab( argv[5] );
				ErrorExplain eexpl(&ab) ;
				p_ab = &ab;
				p_eexpl = &eexpl;
				source_error_byset( argv[4] );
				break; 
			   }
	case 'r' : {
			    char *new_cp = argv[4];
				char *new_mp = argv[5];
				char *p2 = argv[1] + 2;
			    int max_deg = atoi( p2, 10 );
				int amount = 0;
			    if(al_model->check_preprocess() ) {
			        //ALRCCModel m = al_model->delete_node_erasable(max_deg);
			        //ALRCCModel m = al_model->delete_isolate_node(); 
					delete_vex_erasable( max_deg, new_cp, new_mp );
					//m.export_to_file( new_cp, new_mp ) ;
					//amount = al_model->vexNum() - m.vexNum();
					//cout << "delete " << amount << " node " << endl;
			    } else {
				    cout << "sorry the remove erasable procedure could not be execute before preprocess" << endl;
			    }
				break;
			   }
	case 'p' : {
				char *new_cp = argv[4];
				char *new_mp = argv[5];
				ALRCCModel m = al_model->preprocess() ;
				if(m.check_preprocess()) {
					m.export_to_file(new_cp, new_mp);
				} else {
					cout  << "sorry there is inconsistency in the spatial kb " << endl;
				}
				break;
			   }
	default : {
				
				cout << " four parameter styles\n"   \
 "	-m csp_path map_path : check multi definition\n"   \
 "	-i csp_path map_path :  check inverse error\n"   \
 "	-sn csp_path map_path : segmentation by relation n\n"   \
 "	-d csp_path map_path : delete isolate vertex\n"   \
 "	-e csp_path map_path sets_path translattion_item_path : source error by sets file\n"   \
 "	-rn csp_path map_path new_csp_path new_map_path: remove the node erasable\n"   \
 "	-p csp_path map_path new_csp_path new_map_path: preprocess the model\n"  << endl; 
			
				cout << "parameter error!\n";
				return;
			   }
	}

}
#endif


