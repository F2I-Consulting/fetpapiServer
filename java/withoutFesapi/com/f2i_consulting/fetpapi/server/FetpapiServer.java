/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
package com.f2i_consulting.fetpapi.server;

import com.f2i_consulting.fetpapi.etp.Server;

public class FetpapiServer {

	/**
	 * Loading the FesapiCpp native library
	 */
	static {
		try {
			System.loadLibrary("Fetpapi.0.1.0.0");
		}
		catch (UnsatisfiedLinkError e) {
			System.out.println("UnsatisfiedLinkError : " + e.toString());
		}
	}

	public static void main(String[] args) {
		MyServerInitializationParameters initParams = new MyServerInitializationParameters("5c7ed7bc-10dc-47f5-93e4-1de0421a5156", "127.0.0.1", 8080);
		Server etpServer = new Server();
		etpServer.listen(initParams, 2);
		
		System.out.println("FINISHED");
	}
}
