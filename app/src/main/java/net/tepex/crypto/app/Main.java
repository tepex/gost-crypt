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

package net.tepex.crypto.app;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class Main extends Activity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        TextView tv = new TextView(this);
        tv.setText(stringFromJNI());
        setContentView(tv);
    }

    public native String  stringFromJNI();

    //public native String  unimplementedStringFromJNI();

    static
    {
    	System.loadLibrary("streebog");
    }
}
