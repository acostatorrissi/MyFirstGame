// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

void AMyHUD::set_bar_value_percent(float const value)
{
	progress_value->SetPercent(value);
}