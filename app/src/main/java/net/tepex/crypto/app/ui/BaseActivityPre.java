/**
 * Created on: Jun 24, 2016
 *
 * Copyright 20016 Tepex <tepex@mail.ru>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Tepex <tepex@mail.ru>
 */

package net.tepex.crypto.app.ui;

import android.app.ProgressDialog;

import android.content.DialogInterface;
import android.content.Intent;

import android.os.Bundle;

import android.support.annotation.CallSuper;
import android.support.annotation.LayoutRes;
import android.support.annotation.StringRes;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import android.util.Log;

import android.view.MenuItem;

import android.widget.Spinner;

import net.tepex.crypto.app.R;


/**
 * Одно Activity для всех операций.
 */
public class BaseActivityPre extends AppCompatActivity
{
	@CallSuper
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		//return isUpButtonPressed(this, item);
		return true;
	}
	
	@CallSuper
	@Override
	public void onBackPressed()
	{
		super.onBackPressed();
	}
	
	@CallSuper
	@Override
	public void onSaveInstanceState(Bundle bundle)
	{
		//saveCurrentFragment(this, bundle);
		super.onSaveInstanceState(bundle);
	}
	
	/*
	public void showProgressDialog(@StringRes int title, DialogInterface.OnCancelListener listener)
	{
		cancelProgressDialog();
		progressDialog = ProgressDialog.show(this, getString(title), getString(R.string.progress_wait), true, true, listener);
	}
	
	public void cancelProgressDialog()
	{
		if(progressDialog != null)
		{
			progressDialog.dismiss();
			progressDialog = null;
		}
	}
	*/
	
	public void showAlertDialog(@StringRes int title, String msg)
	{
		//cancelProgressDialog();
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(msg)
			.setTitle(title)
			.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int id)
				{
					alertDialog.dismiss();
				}
			});
		
		alertDialog = builder.create();
		alertDialog.show();
	}
	
	protected Toolbar toolbar;
	protected ProgressDialog progressDialog;
	protected AlertDialog alertDialog;
}