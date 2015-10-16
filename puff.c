#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "forest.h"
#include "huffile.h"
#include "tree.h"
#include <ctype.h>

// first and last symbols to dump
#define FIRST '!'
#define LAST '~'

// number of columns to use
#define COLS 8

  
 Tree *tree=NULL; 
int main(int argc, char* argv[])
{
FILE* outputfile;
Forest *forestpointer=NULL;
int count=0; 
Huffeader h;


   
    // ensure proper usage
    if (argc != 3)
    {
       
        printf("Usage:Name of file to puff and name of output file\n" );
        return 1;
    
      
    }
    
  
    
 

    Huffile* inptr;
    inptr= hfopen(argv[1], "r");
    
    if (inptr == NULL )
    {
        printf("Could not open %s for reading.", argv[1]);
        printf("\n");
        return 1;
    }  
 
  

   
   
   
   
     
   
   // open output file
   outputfile = fopen(argv[2], "w");
if (outputfile == NULL)
    {
        printf("Could not open for writing.\n");
        return 1;
    }
      
  
     
   if(hread(&h,inptr)==false)
   {
        hfclose(inptr);
        printf("Could not read header.\n"); 
        return 1;
   
    };
     
  
 
     
   if (h.magic != 0x46465548)
    {
        hfclose(inptr);
        printf("File was not huffed.\n");
        return 1;
    }

    forestpointer=mkforest();
    int checksum = h.checksum;
    for (int i = 0; i < SYMBOLS; i++)
    {
        checksum -= h.frequencies[i];
    }

     if (checksum != 0)
    {
        hfclose(inptr);
        printf("File was not huffed.\n");
        return 1;
    }

   
   
   
 
      for(int i = 0; i < SYMBOLS; i++)
    {
        if(h.frequencies[i] >= 1)
        {
            Tree* tree = mktree();
            tree->symbol = i;
            tree->frequency = h.frequencies[i];
            tree->left = NULL;
            tree->right = NULL;
            plant(forestpointer, tree);
            count=count+1;
            
            
        }
     }
   
   
    Tree* temp1=NULL;
    Tree* temp2=NULL;
    bool done = false;
    while (!done)
    {
      
        
          temp1 = pick(forestpointer);
          temp2 = pick(forestpointer);
        // if there is only one tree left in the forest, a is the huffman tree
        if (temp2 == NULL)
        {
            done = true;
            tree = temp1;
        }
        
      
        else
        {
        

            // combine the two trees 
              Tree *temp= mktree();
              temp->frequency = temp1->frequency + temp2->frequency;
              temp->left = temp1;
              temp->right = temp2;
              plant(forestpointer, temp);
           
           
        }


    }

 // makes two trees the children of a new treenode
      
        int bit;
        
        Tree *Current=tree;
     while((bit=bread(inptr))!=EOF)
     
     
     {
      if (bit==0)
         {
         Current=Current->left;
         }
     
       else if(bit==1)
         {
          Current=Current->right;
         }
        
        if((Current->left==NULL) && ( Current->right==NULL))
         {
         
          fprintf(outputfile, "%c", Current->symbol);
         
          Current=tree;
          
         }
     }  
   
   
      
    // free root
    rmtree(tree);
    rmforest(forestpointer);
    hfclose(inptr);
    fclose(outputfile);
    return 0;   
  } 
       
         
   
   
        
 
    
    
    
    
 
 
   
     
     
     
 
                
     
    
     
    
     
        
   
     
     
                 
     
  
  
  
  
  
  
  
    
    
    
    
