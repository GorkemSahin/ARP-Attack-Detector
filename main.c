#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define xstr(s) str(s)
#define str(s) #s

#define ARP_CACHE       "/proc/net/arp"
#define ARP_STRING_LEN  1023
#define ARP_BUFFER_LEN  (ARP_STRING_LEN + 1)
#define ADDR_LEN 100

#define NETWORK_SIZE 100

#define ARP_LINE_FORMAT "%" xstr(ARP_STRING_LEN) "s %*s %*s " \
                        "%" xstr(ARP_STRING_LEN) "s %*s " \
                        "%" xstr(ARP_STRING_LEN) "s"

int main()
{
	char* ipAddresses[NETWORK_SIZE];
	char* hwAddresses[NETWORK_SIZE];
	int i,j;
    FILE *arpCache = fopen(ARP_CACHE, "r");
    if (!arpCache)
    {
        perror("Arp Cache: Failed to open file \"" ARP_CACHE "\"");
        return 1;
    }

    // Skip header
    char header[ARP_BUFFER_LEN];
    if (!fgets(header, sizeof(header), arpCache))
    {
        return 1;
    }

	// Read and save records from ARP cache.
    char ipAddr[ARP_BUFFER_LEN], hwAddr[ARP_BUFFER_LEN], device[ARP_BUFFER_LEN];
    int count = 0;
    while (3 == fscanf(arpCache, ARP_LINE_FORMAT, ipAddr, hwAddr, device))
    {
    	ipAddresses[count] = (char*) malloc (ADDR_LEN * sizeof(char));
    	hwAddresses[count] = (char*) malloc (ADDR_LEN * sizeof(char));
    	strcpy(ipAddresses[count], ipAddr);
    	strcpy(hwAddresses[count], hwAddr);
        printf("%03d: Mac Address of [%s] on [%s] is \"%s\"\n", ++count, ipAddr, device, hwAddr);
    }
    
    for (i=0;i<count-1;i++){
    	for (j=i+1;j<count;j++){
    		if (strcmp(ipAddresses[i], ipAddresses[j]) == 0){
    			if (strcmp(hwAddresses[i], hwAddresses[j]) != 0){
    				printf("You are being attacked!\n");
				}
			}
		}
	}
    
    fclose(arpCache);
    return 0;
}
