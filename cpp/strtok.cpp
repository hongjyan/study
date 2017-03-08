#include <string.h>
#include <stdio.h>
#include <stdexcept>

int main()
{
   printf("entry main() \r\n");
   char s[] = "Golden Global   View  disk           desk";
   const char *d = " ";
   char *p;
   char t[10][100];
   p = strtok(s,d);
   int i =0 ;

   while(p)
   {
      //Way1:
      /*
      if (0 > snprintf(t[i],sizeof(p), "%s",p)) {
          printf("wrong snprintf\n");
      }
      */
      //Way2:
      strncpy(t[i], p, 100);
      //printf("t[%d]:%s\r\t",i,t[i]); //\r\t will let undefine beahivor
      printf("t[%d]:%s\n",i,t[i]); //OK
      } catch (std::exception& e) {
          printf("%s\n", e.what());
      }
      i++ ;
      p=strtok(NULL,d);
                                                                                            }

      return 0;
}
