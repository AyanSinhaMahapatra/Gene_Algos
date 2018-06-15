//Test Functions Here for Tree Search 

void recursive_trav(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd array_val,int rec_level)
{
	if(rec_level == level_limit)
		return;

	Eigen::VectorXd array(116);
	array = array_val;
	random_change(coeff_mat,array);

	int temp_cost = cost_full(coeff_mat,array);
	//cout<<"Cost at rec == "<<temp_cost<<endl;
	check_history_and_store(coeff_mat,array);

	omp_set_num_threads(NUM_THREADS);

	double start_time = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp parallel for
		for(int i=1;i<=no_of_rec;i++){
			recursive_trav(coeff_mat,array,rec_level+1);
		}
	}
	double run_time = omp_get_wtime() - start_time;
	//cout<<"Time taken == "<<run_time<<endl;

	return;
}


/*

void check_and_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test,Eigen::MatrixXd& coeff_mat)
{
	if(cost_after==0)
	{
		cout<<"--------------------------------One Result Found-------------------------------"<<endl;
		print_result_new(array);
		cout<<endl<<array<<endl;
		flag_test_2=0;
		//result_flag=1;
		//return;
	}
	else{
		cout<<"--------------------------------Stuck Somewhere-------------------------------"<<endl;
		cout<<"Cost = "<<cost_after<<endl<<endl;

		// Swapper Stuff Goes Here
		//cout<<" Swapper Mode Enabled "<<endl;
		flag_swap=1;
		while((cost_after!=0)&&(flag_swap!=0))
		{
			swapper_gen=1;
			if(perform_swap(array,array_test))
			{
				cost_after=cost_full(coeff_mat,array);   //Changed Here
				cout<<" Swap Not Reducing Cost -- ERROR ERROR"<<endl;
				if(cost_after<=cost_main)
				{
					cost_main=cost_after;
					//print_result_lowest(cost_main,array);
				}
				flag_swap=0;
				//cout<<"Mutation Average = "<<float(mutation_total/count_2)/2<<endl;
				//cout<<"Average Changes by Genetic Algo in One go == "<<(changes_total/(count_2*2))<<endl;
				cout<<"Cost Here Is, After Swapper --  "<<cost_after<<endl;
				cout<<"Lowest Cost Till Now Is  --  "<<cost_main<<endl; 
			}
			cost_after=cost_full(coeff_mat,array);
			cout<<"After Performing "<<swapper_gen<<" of Batches of Swap, Cost is  -  "<<cost_after<<endl;
			if(cost_after==0)
			{
				cout<<"------------O Cost Achieved-------------"<<endl;
				cost_main=0;
				print_result_new(array); //Print To File 
			}
		}
					flag_test_2=0;
	}
}

int perform_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test)
{
	Eigen::MatrixXd coeff_mat_in_swap(116,116);
	assign_coeff_mat(coeff_mat_in_swap);
	int cost_before=0;
	int cost_afterr=0;
	int temp = 0;
	int first_cost=cost_full(coeff_mat_in_swap,array);
	array_test=array;
	//Loops
	for(int i=2;i<115;i++)
	{
		if(i==94)
			i++;
		//cout<<"I == "<<i<<endl;
		for(int j=2;j<115;j++)
		{
			if(j==94)
				j++;

			if(array(i)!=array(j))
			{
				//swaps_performed++;
				cost_before = cost_full(coeff_mat_in_swap,array);
				//cout<<"Cost Before == "<<cost_full(coeff_mat,array)<<endl;
				//Swap I and J 
				temp = array(i);
				array(i)=array(j);
				array(j)=temp;
				//cout<<"Cost After == "<<cost_full(coeff_mat,array)<<endl;
				//Compute Cost
				cost_afterr = cost_full(coeff_mat_in_swap,array);
				//cost_sum+=cost;
				if(cost<cost_lowest)
					cost_lowest=cost;
				if(cost>cost_highest)
				//	cost_highest=cost;

				//Reswap
				if(cost_afterr>cost_before)
				{
					temp = array(i);
					array(i)=array(j);
					array(j)=temp;
					cost_afterr=cost_before;
				}
				//else
					//cout<<endl<<" 1 Swapping Effective "<<endl;
			}
		}
	}

	//cout<<"Swapped"<<endl;
	if(cost_afterr == first_cost)
		return 1;  // This Has to Be Swap Not Reducing Cost 
	else 
		return 0;
}

void shake(Eigen::VectorXd& array_main,Eigen::VectorXd& array,int k_neighbourhood)
{
	//cout<<"Stirred Not Shaken"<<endl;
	int array_loc1[57];
	int array_loc0[57];
	int array_random1[k_neighbourhood+1];
	int array_random0[k_neighbourhood+1];
	int pointer_0=0;
	int pointer_1=0;
	int rand_gen=0;
	for(int i=1;i<=k_neighbourhood;i++)
	{
		array_random1[i]=0;
		array_random0[i]=0;
	}

	for(int i=2;i<=114;i++)
	{
		if(i==94)
			i++;
		if(array_main(i)==0)
		{
			pointer_0++;
			array_loc0[pointer_0] = i; 
		}
		else if(array_main(i)==1)
		{
			pointer_1++;
			array_loc1[pointer_1] = i;
		}
	}
	if((pointer_0==56)&&(pointer_1==56))
	{
		//cout<<" First Task Okay "<<endl;
	}
	else
	{
		cout<<" Error : Pointers at :"<<endl;
		cout<<pointer_1<<" == "<<pointer_0<<endl;
	}


	for(int i=1;i<=k_neighbourhood;i++)
	{
		rand_gen = rand() % 56;
		rand_gen++;
		while(check_array(rand_gen,array_random1,k_neighbourhood)==0)
		{
			rand_gen = rand() % 56;
			rand_gen++;
		}
		array_random1[i]=rand_gen;
		rand_gen = rand() % 56;
		rand_gen++;
		while(check_array(rand_gen,array_random0,k_neighbourhood)==0)
		{
			rand_gen = rand() % 56;
			rand_gen++;
		}
		array_random0[i]=rand_gen;
	}

	array=array_main;

	for(int i=1;i<=k_neighbourhood;i++)
	{
		array(array_loc1[array_random1[i]])=0;
		array(array_loc0[array_random0[i]])=1;
	}

	if(trench_no%2==0)   //For Alternates Of Fixed Positions 94 and 115 
		swap_fixed_pos(array);
}

void print_vns(int flag_random,int flag_neigh,int flag_tren,int flag_shake,int random_run_no,int neighbourhood_no,int trench_no,
	int shake_no,int cost_trench,int cost_shake)
{
	FILE *fp_log;
	fp_log = fopen("vns_log_orig.txt","a"); 
	if(flag_random==1)
	{
		cout<<"***********************************************************************************"<<endl;
		cout<<"***********************************************************************************"<<endl;
		cout<<"***********************************************************************************"<<endl;
		cout<<"                              Random Run Number == "<<random_run_no<<endl;
		cout<<"Initial Cost Of Randomly Generated Array = "<<cost_shake<<endl;
		fprintf(fp_log,"                              Random Run Number == %d \n",random_run_no);
	}
	if(flag_neigh==1)
	{
		cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"                   Neighbourhood Number == "<<neighbourhood_no<<endl;
		fprintf(fp_log,"                   Neighbourhood Number == %d \n",neighbourhood_no);
	}
	if(flag_tren==1)
	{
		cout<<"=================================================="<<endl;
		cout<<"        Trench Number =="<<trench_no<<"  Cost = "<<cost_trench<<endl;
		fprintf(fp_log,"        Trench Number == %d  Cost = %d \n",trench_no,cost_trench);
	}
	if(flag_shake==1)
	{
		
		cout<<"Shake Number ::::::::: "<<shake_no<<"   Cost = "<<cost_shake<<endl;
		fprintf(fp_log,"Shake Number ::::::::: %d  Cost = %d \n",shake_no,cost_shake);
		if(cost_shake<cost_trench)
		{
			cout<<"<<<<<<<<<<<<<<<<<<<<Lower Cost Found.>>>>>>>>>>>>>>>>>>>>>"<<endl<<" Cost == "<<cost_shake<<endl;
			fprintf(fp_log,"<<<<<<<<Lower Cost Found.>>>>>>>>>>> Cost == %d \n",cost_shake);
		}
	}
	if((flag_random==0)&&(flag_neigh==0)&&(flag_tren==0)&&(flag_shake==0))
	{
		cout<<"--------------------------------One Result Found-------------------------------"<<endl;
		fprintf(fp_log,"--------------------------------One Result Found------------------------------- \n");
	}
	fclose(fp_log);
}

void print_result_new(Eigen::VectorXd& array)
{
	FILE* fp_rslt;
	fp_rslt=fopen("results_found_by_me.txt","a");
	int temp;
	cout<<endl;
	for(int i=1;i<116;i++)
	{
		temp=array(i);
		cout<<temp;
		fprintf(fp_rslt,"%d",temp);
	}
	cout<<endl;
	fprintf(fp_rslt,"\n");
	fclose(fp_rslt);
}
void print_result_lowest(int cost,Eigen::VectorXd& array)
{
	FILE* fp_rslt_low;
	fp_rslt_low=fopen("results_found_by_me_lowest_org.txt","a+");
	int temp;
	cout<<endl;
	for(int i=1;i<116;i++)
	{
		temp=array(i);
		cout<<temp;
		fprintf(fp_rslt_low,"%d",temp);
	}
	fprintf(fp_rslt_low, " Cost : %d ",cost);
	cout<<endl;
	fprintf(fp_rslt_low,"\n");
	fclose(fp_rslt_low);
}

void swap_fixed_pos(Eigen::VectorXd& array)
{
	array(94)=1-array(94);
    array(115)=1-array(115);
}

void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat)
{
	int swap_cost_1,swap_cost_2;
	swap_cost_1 = cost_full(coeff_mat,array);
	swap_fixed_pos(array);
	swap_cost_2 = cost_full(coeff_mat,array);
	if(swap_cost_1<swap_cost_2)
		swap_fixed_pos(array);
}

void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results) 
{
    FILE* fp_diff;
    fp_diff = fopen("diff_log_2.txt","a"); 
    Eigen::VectorXd result(116);
    int diff = 0;
    int temp;
    for(int i=1;i<=4;i++)
    {
       result = results.row(i); 
       diff = change_in_array(result,array); 
       if(diff<lowest_differance)
            {
                lowest_differance=diff;
                for(int j=1;j<=115;j++)
                {
                    temp = array(j);
                    fprintf(fp_diff, "%d",temp);
                }
                fprintf(fp_diff,"\n  Difference == %d  \n",lowest_differance);
            }
    }
    fclose(fp_diff);
}

int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test)
{
	int changes=0;
	for(int i=2;i<116;i++)
	{
		if(array(i)!=array_test(i))
			changes++;
	}
	return changes;
}

int check_array(int rand_gen,int *array_random,int size)
{
	int flag = 1; 
	//cout<<"Size = "<<size<<endl;
	for(int i=1;i<=size;i++)
	{
		if(array_random[i]==rand_gen)
			flag=0;
	}
	return flag;
}

void vns_recursive(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{
	Eigen::VectorXd array_temp(116);
	array_temp = array;

	int cost_vns = 0;
	while(cost_main!=0)
	{
		shake(array,array_temp,neighbourhood_no);  // Shaked To K'th Neighbourhood
		array = array_temp;
		lowest_cost = cost_full(coeff_mat,array);
		cout<<"Cost Before Shaking == "<<cost_main<<endl;
		cout<<"Cost After Shaking == "<<lowest_cost<<endl;

		// Local Search Starts Here
		flag_test_2=1;
		count_2=0;
		while(flag_test_2)
		{
			count_2++;

			recursive_trav(coeff_mat,array,1);   // Does one iteration of Tree Search
		
			assign_array(array);
			try_swap_fixed(array,coeff_mat);
			cost_after=cost_full(coeff_mat,array);

			if((cost_after<lowest_cost)||((count_2%(genetic_till/10))==0))
			{

				cout<<"Cost at Generation "<<count_2<<" = "<<cost_after<<endl; 
				lowest_cost=cost_after;
			}
			if((count_2%genetic_till)==0)
				check_and_swap(array,array_temp,coeff_mat);
		}
		// Till Here Local Search 

		if(is_okay(array)==0)
		{
			cout<<"Problem In Array : Fatal Error"<<endl;
			return;
		}

		cost_vns = cost_full(coeff_mat,array);
		cout<<"Cost After Local Search : "<<cost_vns<<endl;

		if(cost_vns<cost_main)
		{
			print_vns(0,0,0,1,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
			for(int index = 0;index<=115;index++)
				array_main[index] = array(index);

			cost_main = cost_vns;
			trench_no++;
			shake_no = 1;
			print_vns(0,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
			print_result_lowest(cost_main,array);
		}
		else
		{	
			if(cost_vns<13)
				print_result_lowest(cost_vns,array);
				//try_difference(array,results_old);   //Fill Here
				cout<<"Numbers = "<<random_run_no<<" "<<neighbourhood_no<<" "<<trench_no<<" "<<shake_no<<"Cost == "<<cost_vns<<endl;
				shake_no++;
		}

		if(shake_no>=max_shake) //max_shake = 20
		{
			//shake(array_main,array,neighbourhood_no);  // Shaked To K'th Neighbourhood
			lowest_cost = cost_full(coeff_mat,array);
			trench_no++;
			shake_no = 1;
			print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
		}

		if(trench_no>=max_trench) //max_trench = 10
		{
			neighbourhood_no++;
			trench_no = 1;
			shake_no = 1;
			print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
		}

		if(neighbourhood_no>=max_neigh) //max_neigh = 20
		{
			neighbourhood_no = 1;
			trench_no = 1;
			shake_no = 1; 
			random_run_no++;
			//array_main = results_new.row(random_run_no);
			random_generate(array);
			cost_main = cost_full(coeff_mat,array);
			print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,0,cost_main);
		}
	}	

	return;
}*/